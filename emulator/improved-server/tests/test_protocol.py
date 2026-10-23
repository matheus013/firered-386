"""Protocol-level integration tests, run against either a Windows or Linux build.

Clients model the original 1.72 wire format. These do not substitute for testing
actual VBA Link instances, FireRed trades or timing-sensitive emulation.
"""
import argparse
import contextlib
from pathlib import Path
import queue
import socket
import struct
import subprocess
import sys
import threading
import time
import unittest

PARSER = argparse.ArgumentParser()
PARSER.add_argument("server", type=Path)
ARGS, UNITTEST_ARGS = PARSER.parse_known_args()
SERVER = str(ARGS.server.resolve())
CREATION_FLAGS = getattr(subprocess, "CREATE_NO_WINDOW", 0)


def receive(sock, size):
    result = bytearray()
    while len(result) < size:
        block = sock.recv(size - len(result))
        if not block:
            raise AssertionError(f"Unexpected EOF: {len(result)}/{size} bytes")
        result.extend(block)
    return bytes(result)


def fragmented(sock, data):
    for byte in data:
        sock.sendall(bytes([byte]))
        time.sleep(0.001)


class Session:
    def __init__(self, players, timeout=2000):
        self.players = players
        self.sockets = []
        self.lines = []
        self.output = queue.Queue()
        self.process = subprocess.Popen(
            [SERVER, "--players", str(players), "--port", "0", "--timeout-ms", str(timeout)],
            stdin=subprocess.DEVNULL, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
            text=True, creationflags=CREATION_FLAGS,
        )
        def reader():
            for line in self.process.stdout:
                self.lines.append(line.rstrip())
                self.output.put(line.rstrip())
        self.thread = threading.Thread(target=reader, daemon=True)
        self.thread.start()
        try:
            line = self.output.get(timeout=5)
            assert line.startswith("LISTENING 127.0.0.1 "), line
            self.port = int(line.split()[-1])
        except BaseException:
            self.close()
            raise

    def connect(self):
        try:
            for _ in range(self.players):
                self.add_player()
            for player, sock in enumerate(self.sockets):
                assert receive(sock, 4) == bytes([player, 0, 0, 0])
            return self
        except BaseException:
            self.close()
            raise

    def add_player(self):
        sock = socket.create_connection(("127.0.0.1", self.port), timeout=3)
        sock.settimeout(3)
        self.sockets.append(sock)
        return sock

    def expect_exit(self, code):
        actual = self.process.wait(timeout=4)
        error = self.process.stderr.read()
        assert actual == code, (actual, code, error, self.lines)
        self.thread.join(timeout=1)

    def stop(self, player):
        self.sockets[player].sendall(b"S" + bytes(7 if player == 0 else 3))
        self.expect_notifications(player)
        self.expect_exit(0)

    def expect_notifications(self, skip):
        for player, sock in enumerate(self.sockets):
            if player == skip: continue
            packet = receive(sock, 4 * (self.players - 1 if player == 0 else self.players))
            assert packet == b"S" + bytes(len(packet) - 1), (player, packet)

    def close(self):
        for sock in self.sockets:
            with contextlib.suppress(OSError): sock.close()
        if self.process.poll() is None:
            self.process.terminate()
            try: self.process.wait(timeout=3)
            except subprocess.TimeoutExpired:
                self.process.kill()
                self.process.wait(timeout=3)
        self.thread.join(timeout=1)
        self.process.stdout.close()
        self.process.stderr.close()

    def __enter__(self): return self
    def __exit__(self, *_): self.close()


def host_frame(round_number):
    return bytes([ord("M"), 3, round_number & 255, 0x12]) + struct.pack(">I", 0x80000000 | round_number)


def reply_frame(player, round_number):
    return bytes([ord("M"), 3 | (player << 6), round_number & 255, 0xa0 + player])


def exchange(session, rounds=3, partial=False, coalesced=False):
    previous = {p: bytes([ord("M"), 3 | (p << 6), 255, 255]) for p in range(1, session.players)}
    frames = [host_frame(i) for i in range(rounds)]
    if coalesced: session.sockets[0].sendall(b"".join(frames))
    for index, host in enumerate(frames):
        if not coalesced:
            (fragmented if partial else socket.socket.sendall)(session.sockets[0], host)
        for player in range(1, session.players):
            order = [p for p in range(1, session.players) if p != player]
            if session.players == 4 and player == 3: order = [2, 1]
            expected = host + b"".join(previous[p] for p in order)
            assert receive(session.sockets[player], 4 * session.players) == expected
        replies = []
        for player in range(1, session.players):
            reply = reply_frame(player, index)
            (fragmented if partial else socket.socket.sendall)(session.sockets[player], reply)
            replies.append(reply)
            previous[player] = reply
        assert receive(session.sockets[0], 4 * (session.players - 1)) == b"".join(replies)


class ProtocolTests(unittest.TestCase):
    def test_fragmented_and_coalesced_frames(self):
        for players in (2, 3, 4):
            with self.subTest(players=players, mode="fragmented"):
                with Session(players).connect() as session:
                    exchange(session, partial=True)
                    session.stop(0)
            with self.subTest(players=players, mode="coalesced"):
                with Session(players).connect() as session:
                    exchange(session, rounds=12, coalesced=True)
                    session.stop(0)

    def test_any_player_can_stop_while_host_is_idle(self):
        for players in (2, 3, 4):
            for player in range(players):
                with self.subTest(players=players, player=player):
                    with Session(players).connect() as session:
                        session.stop(player)

    def test_any_player_disconnects_without_spinning(self):
        for players in (2, 3, 4):
            for player in range(players):
                with self.subTest(players=players, player=player):
                    with Session(players).connect() as session:
                        session.sockets[player].close()
                        session.expect_notifications(player)
                        session.expect_exit(1)

    def test_disconnect_halfway_through_reply(self):
        with Session(3).connect() as session:
            session.sockets[0].sendall(host_frame(0))
            for sock in session.sockets[1:]: receive(sock, 12)
            session.sockets[1].sendall(b"M\x43")
            session.sockets[1].close()
            session.expect_notifications(1)
            session.expect_exit(1)

    def test_configurable_timeout(self):
        with Session(2, timeout=250).connect() as session:
            session.expect_notifications(-1)
            session.expect_exit(1)

    def test_disconnect_before_all_players_arrive(self):
        with Session(3) as session:
            session.add_player().close()
            session.expect_exit(1)

    def test_invalid_arguments_and_eof(self):
        for arguments in (["--players", "1"], ["--players", "5"], ["--players", "3x"],
                          ["--players", "3", "--port", "65536"], ["--players", "3", "--timeout-ms", "-1"],
                          ["--players", "3", "--bind", "not-an-ip"], ["--wat"], []):
            with self.subTest(arguments=arguments):
                result = subprocess.run([SERVER] + arguments, input="", capture_output=True,
                                        text=True, timeout=3, creationflags=CREATION_FLAGS)
                self.assertNotEqual(result.returncode, 0)


if __name__ == "__main__":
    unittest.main(argv=[sys.argv[0]] + UNITTEST_ARGS, verbosity=2)
