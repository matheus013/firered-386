// SPDX-License-Identifier: GPL-2.0-or-later
#include "vba-link/src/LinkTransport.h"
#include <cassert>
#include <cstring>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <vector>

struct Pair {
    int fd[2];
    Pair() { assert(socketpair(AF_UNIX, SOCK_STREAM, 0, fd) == 0); }
    ~Pair() { close(fd[0]); close(fd[1]); }
};
int main() {
    // All valid packet sizes, fragmented byte by byte.
    for (int length = 4; length <= 12; length += 2) {
        Pair p;
        std::vector<char> frame(length, 'x'); frame[0] = length;
        std::thread writer([&] {
            for (char byte : frame) {
                assert(send(p.fd[1], &byte, 1, MSG_NOSIGNAL) == 1);
                usleep(1000);
            }
        });
        char data[16] = {};
        assert(link_transport::receive_frame(p.fd[0], data, sizeof(data), 500));
        assert(memcmp(data, frame.data(), length) == 0);
        writer.join();
    }
    // Fragmented prefix followed by coalesced end + next frame.
    {
        Pair p; char frames[20] = {8, 1, 2, 3, 4, 5, 6, 7, 12, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
        std::thread writer([&] {
            assert(send(p.fd[1], frames, 3, MSG_NOSIGNAL) == 3);
            usleep(10000);
            assert(send(p.fd[1], frames + 3, 17, MSG_NOSIGNAL) == 17);
        });
        char data[16];
        assert(link_transport::receive_frame(p.fd[0], data, sizeof(data), 500));
        assert(memcmp(data, frames, 8) == 0);
        assert(link_transport::receive_frame(p.fd[0], data, sizeof(data), 500));
        assert(memcmp(data, frames + 8, 12) == 0);
        writer.join();
    }
    // Every invalid length byte, plus a too-small destination.
    for (int length = 0; length <= 255; ++length) {
        if (length >= 4 && length <= 12 && length % 2 == 0) continue;
        Pair p; char first = static_cast<char>(length), data[16];
        assert(send(p.fd[1], &first, 1, MSG_NOSIGNAL) == 1);
        assert(!link_transport::receive_frame(p.fd[0], data, sizeof(data), 50));
    }
    {
        Pair p; char frame[12] = {12}, data[4];
        assert(send(p.fd[1], frame, sizeof(frame), MSG_NOSIGNAL) == 12);
        assert(!link_transport::receive_frame(p.fd[0], data, sizeof(data), 50));
    }
    // EOF before/after a partial frame, and timeout with no input/partial input.
    for (int partial = 0; partial <= 1; ++partial) {
        for (int eof = 0; eof <= 1; ++eof) {
            Pair p; char first = 8, data[16];
            if (partial) assert(send(p.fd[1], &first, 1, MSG_NOSIGNAL) == 1);
            if (eof) shutdown(p.fd[1], SHUT_WR);
            auto start = link_transport::Clock::now();
            assert(!link_transport::receive_frame(p.fd[0], data, sizeof(data), 30));
            assert(link_transport::Clock::now() - start < std::chrono::seconds(1));
        }
    }
    // Handshake stays separate from a coalesced ready packet.
    {
        Pair p; char bytes[8] = {1, 0, 2, 0, 4, 0, 0, 0}, data[8];
        assert(send(p.fd[1], bytes, 8, MSG_NOSIGNAL) == 8);
        assert(link_transport::receive_exact(p.fd[0], data, 4, 50));
        assert(memcmp(data, bytes, 4) == 0);
        assert(link_transport::receive_frame(p.fd[0], data, 8, 50));
        assert(memcmp(data, bytes + 4, 4) == 0);
    }
    // Large nonblocking writes exercise retry/short-write handling.
    {
        Pair p; int size = 1024;
        assert(setsockopt(p.fd[0], SOL_SOCKET, SO_SNDBUF, &size, sizeof(size)) == 0);
        std::vector<char> source(1024 * 1024, 'q'), result(source.size());
        std::thread reader([&] { assert(link_transport::receive_exact(p.fd[1], result.data(), result.size(), 3000)); });
        assert(link_transport::send_exact(p.fd[0], source.data(), source.size(), 3000));
        reader.join(); assert(source == result);
    }
    // Broken pipe is returned as failure and does not terminate the process.
    {
        Pair p; shutdown(p.fd[1], SHUT_RDWR); char data = 0;
        assert(!link_transport::send_exact(p.fd[0], &data, 1, 50));
    }
    std::cout << "PASS: 265 transport scenarios (fragmentation, coalescing, invalid lengths, EOF, deadlines, short writes, broken pipe)\n";
}
