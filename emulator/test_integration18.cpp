// SPDX-License-Identifier: GPL-2.0-or-later
// Compile with function sections and --gc-sections to isolate actual LAN methods.
#include "vba-link/src/GBALink.cpp"
#include <cassert>
#include <iostream>
#include <thread>

struct Sockets {
    int fd[2];
    Sockets() { assert(socketpair(AF_UNIX, SOCK_STREAM, 0, fd) == 0); }
    ~Sockets() { close(fd[0]); close(fd[1]); }
};
int main() {
    linktimeout = 100;
    for (int players = 2; players <= 4; ++players) {
        lanlink.numgbas = players - 1; lanlink.type = 0; linkid = 1;
        Sockets p; lanlink.tcpsocket = p.fd[0];
        char frames[24] = {};
        int length = 8 + 2 * (players - 2);
        for (int round = 0; round < 2; ++round) {
            char* frame = frames + round * length;
            frame[0] = length; frame[1] = 3; frame[2] = 10 + round;
            frame[4] = 42;
            if (players >= 3) frame[8] = 22 + round;
            if (players >= 4) frame[10] = 33 + round;
        }
        // Run the real CheckConn + Recv across a fragmented/coalesced boundary.
        assert(send(p.fd[1], frames, 3, MSG_NOSIGNAL) == 3);
        std::thread writer([&] {
            usleep(5000);
            assert(send(p.fd[1], frames + 3, 2 * length - 3, MSG_NOSIGNAL) == 2 * length - 3);
        });
        lanlink.connected = true;
        lc.CheckConn();
        assert(lanlink.connected && linkdata[0] == 10 && tspeed == 3);
        if (players >= 3) assert(linkdata[2] == 22);
        if (players >= 4) assert(linkdata[3] == 33);
        lc.Recv(); writer.join();
        assert(lanlink.connected && linkdata[0] == 11 && savedlinktime == 42);
        if (players >= 3) assert(linkdata[2] == 23);
        if (players >= 4) assert(linkdata[3] == 34);
        // Send method keeps original length and participant ID encoding.
        linkdata[1] = 99; lc.Send(); char reply[4];
        assert(link_transport::receive_exact(p.fd[1], reply, 4, 100));
        assert(reply[0] == 4 && reply[1] == 4 && reply[2] == 99);
        shutdown(p.fd[1], SHUT_WR); lc.CheckConn(); assert(!lanlink.connected);
    }
    // Real host Recv drains two queued frames per client and retains the latest.
    for (int players = 2; players <= 4; ++players) {
        Sockets a, b, c;
        Sockets* peers[] = {&a, &b, &c};
        lanlink.numgbas = players - 1; lanlink.connected = true; lanlink.type = 0;
        ls.howmanytimes = 1;
        for (int peer = 0; peer < players - 1; ++peer) {
            ls.tcpsocket[peer + 1] = peers[peer]->fd[0];
            char frames[8] = {4, 0, 1, 0, 4, 0, static_cast<char>(70 + peer), 0};
            assert(send(peers[peer]->fd[1], frames, 8, MSG_NOSIGNAL) == 8);
        }
        ls.Recv(); assert(lanlink.connected && ls.howmanytimes == 0);
        for (int peer = 0; peer < players - 1; ++peer) assert(linkdata[peer + 1] == 70 + peer);
    }
    // Short data frames are rejected before decoding stale bytes.
    {
        Sockets p; lanlink.tcpsocket = p.fd[0]; lanlink.numgbas = 3; lanlink.connected = true;
        char frame[4] = {4, 0, 0, 0};
        assert(send(p.fd[1], frame, 4, MSG_NOSIGNAL) == 4);
        lc.CheckConn(); assert(!lanlink.connected);
    }
    std::cout << "PASS: actual 1.8 client/host methods, 2/3/4 players; fragmented and queued frames, send encoding, EOF and invalid data size\n";
}
