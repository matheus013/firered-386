// SPDX-License-Identifier: GPL-2.0-or-later
// Bounded TCP framing for the Linux VBA Link 1.8 port.
#ifndef VBA_LINK_TRANSPORT_H
#define VBA_LINK_TRANSPORT_H
#include <cerrno>
#include <chrono>
#include <climits>
#include <cstddef>
#include <poll.h>
#include <sys/socket.h>

namespace link_transport {
typedef std::chrono::steady_clock Clock;

inline bool wait(int socket, short events, Clock::time_point deadline) {
    for (;;) {
        long long remaining = std::chrono::duration_cast<std::chrono::milliseconds>(deadline - Clock::now()).count();
        if (remaining < 0) return false;
        pollfd descriptor = {socket, events, 0};
        int result = poll(&descriptor, 1, remaining > INT_MAX ? INT_MAX : static_cast<int>(remaining));
        if (result > 0) return (descriptor.revents & events) != 0;
        if (result < 0 && errno == EINTR) continue;
        return false;
    }
}

inline bool transfer(int socket, char* buffer, std::size_t size, bool sending, Clock::time_point deadline) {
    std::size_t offset = 0;
    while (offset < size) {
        if (!wait(socket, sending ? POLLOUT : POLLIN, deadline)) return false;
        ssize_t count = sending
            ? send(socket, buffer + offset, size - offset, MSG_DONTWAIT | MSG_NOSIGNAL)
            : recv(socket, buffer + offset, size - offset, MSG_DONTWAIT);
        if (count > 0) offset += static_cast<std::size_t>(count);
        else if (count == 0 || (errno != EINTR && errno != EAGAIN && errno != EWOULDBLOCK)) return false;
    }
    return true;
}

inline Clock::time_point deadline(int timeout_ms) {
    return Clock::now() + std::chrono::milliseconds(timeout_ms > 0 ? timeout_ms : 1);
}
inline bool receive_exact(int socket, char* buffer, std::size_t size, int timeout_ms) {
    return transfer(socket, buffer, size, false, deadline(timeout_ms));
}
inline bool send_exact(int socket, const char* buffer, std::size_t size, int timeout_ms) {
    return transfer(socket, const_cast<char*>(buffer), size, true, deadline(timeout_ms));
}
// Each 1.8 LAN frame begins with an unsigned length byte. Read only that
// frame; the following frame stays in the socket even when TCP coalesces it.
inline bool receive_frame(int socket, char* buffer, std::size_t capacity, int timeout_ms) {
    Clock::time_point end = deadline(timeout_ms);
    if (capacity < 4 || !transfer(socket, buffer, 1, false, end)) return false;
    unsigned length = static_cast<unsigned char>(buffer[0]);
    if (length < 4 || length > 12 || length % 2 || length > capacity) return false;
    return transfer(socket, buffer + 1, length - 1, false, end);
}
}
#endif
