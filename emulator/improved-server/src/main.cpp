// SPDX-License-Identifier: GPL-2.0-or-later
// A new relay implementation of the VBALink 1.72 LAN wire protocol.
// Protocol reference: denopqrihg's VBALink sources and server/main.cpp.
// See ../../AUDIT.md for provenance and the limits of compatibility testing.
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
using SocketHandle = SOCKET;
constexpr SocketHandle invalid_socket = INVALID_SOCKET;
#else
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
using SocketHandle = int;
constexpr SocketHandle invalid_socket = -1;
#endif
#include <algorithm>
#include <array>
#include <cerrno>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using Bytes = std::vector<std::uint8_t>;
using Clock = std::chrono::steady_clock;

static int socket_error() {
#ifdef _WIN32
    return WSAGetLastError();
#else
    return errno;
#endif
}
static bool retryable(int error) {
#ifdef _WIN32
    return error == WSAEWOULDBLOCK || error == WSAEINTR;
#else
    return error == EWOULDBLOCK || error == EAGAIN || error == EINTR;
#endif
}
static void close_socket(SocketHandle handle) {
#ifdef _WIN32
    closesocket(handle);
#else
    close(handle);
#endif
}
struct NetworkRuntime {
    NetworkRuntime() {
#ifdef _WIN32
        WSADATA data{};
        if (WSAStartup(MAKEWORD(2, 2), &data) != 0)
            throw std::runtime_error("Winsock initialization failed");
#endif
    }
    ~NetworkRuntime() {
#ifdef _WIN32
        WSACleanup();
#endif
    }
};
class Socket {
public:
    explicit Socket(SocketHandle value = invalid_socket) : value_(value) {}
    ~Socket() { if (value_ != invalid_socket) close_socket(value_); }
    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;
    Socket(Socket&& other) noexcept : value_(other.value_) { other.value_ = invalid_socket; }
    Socket& operator=(Socket&& other) noexcept {
        if (this != &other) {
            if (value_ != invalid_socket) close_socket(value_);
            value_ = other.value_;
            other.value_ = invalid_socket;
        }
        return *this;
    }
    SocketHandle get() const { return value_; }
private:
    SocketHandle value_;
};
static void nonblocking(SocketHandle handle) {
#ifdef _WIN32
    u_long enabled = 1;
    if (ioctlsocket(handle, FIONBIO, &enabled) != 0)
        throw std::runtime_error("Cannot enable nonblocking socket");
#else
    int flags = fcntl(handle, F_GETFL, 0);
    if (flags < 0 || fcntl(handle, F_SETFL, flags | O_NONBLOCK) != 0)
        throw std::runtime_error("Cannot enable nonblocking socket");
#endif
}
static void check_fd(SocketHandle handle) {
#ifndef _WIN32
    if (handle < 0 || handle >= FD_SETSIZE)
        throw std::runtime_error("Socket outside select() descriptor range");
#else
    (void)handle;
#endif
}
static int send_bytes(SocketHandle handle, const std::uint8_t* data, std::size_t size) {
#ifdef VBALINK_TEST_FRAGMENT_SEND
    size = std::min(size, static_cast<std::size_t>(VBALINK_TEST_FRAGMENT_SEND));
#endif
    int flags = 0;
#ifdef MSG_NOSIGNAL
    flags |= MSG_NOSIGNAL;
#endif
    return static_cast<int>(send(handle, reinterpret_cast<const char*>(data), static_cast<int>(size), flags));
}
struct Deadline {
    explicit Deadline(int milliseconds) : enabled(milliseconds != 0), end(Clock::now() + std::chrono::milliseconds(milliseconds)) {}
    bool enabled;
    Clock::time_point end;
    timeval* remaining(timeval& result) const {
        if (!enabled) return nullptr;
        auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(end - Clock::now()).count();
        if (microseconds <= 0) throw std::runtime_error("Session timed out");
        result.tv_sec = static_cast<long>(microseconds / 1000000);
        result.tv_usec = static_cast<long>(microseconds % 1000000);
        return &result;
    }
};
struct Disconnected : std::runtime_error {
    explicit Disconnected(std::size_t player) : std::runtime_error("Player " + std::to_string(player + 1) + " disconnected"), player(player) {}
    std::size_t player;
};
struct StopRequested { std::size_t player; };

// TCP is a byte stream: all reads are accumulated, including coalesced frames.
// Pumping every client also notices a disconnect by a non-active player.
class Transport {
public:
    Transport(std::vector<Socket>& sockets, int timeout_ms)
        : sockets_(sockets), queues_(sockets.size()), timeout_ms_(timeout_ms) {}

    Bytes read_exact(std::size_t player, std::size_t count) {
        Deadline deadline(timeout_ms_);
        check_stops();
        while (queues_[player].size() < count) pump(-1, deadline);
        Bytes data;
        data.reserve(count);
        for (std::size_t i = 0; i < count; ++i) {
            data.push_back(queues_[player].front());
            queues_[player].pop_front();
        }
        return data;
    }
    void write_exact(std::size_t player, const Bytes& data) {
        Deadline deadline(timeout_ms_);
        std::size_t sent = 0;
        while (sent < data.size()) {
            if (!pump(static_cast<int>(player), deadline)) continue;
            int count = send_bytes(sockets_[player].get(), data.data() + sent, data.size() - sent);
            if (count > 0) sent += static_cast<std::size_t>(count);
            else if (count == 0 || !retryable(socket_error())) throw Disconnected(player);
        }
    }
private:
    void check_stops() const {
        for (std::size_t player = 0; player < sockets_.size(); ++player)
            if (!queues_[player].empty() && queues_[player].front() == 'S'
             && queues_[player].size() >= (player == 0 ? 8u : 4u))
                throw StopRequested{player};
    }
    bool pump(int writing, const Deadline& deadline) {
        check_stops();
        fd_set reading, writable;
        FD_ZERO(&reading);
        FD_ZERO(&writable);
        SocketHandle highest = 0;
        for (const auto& socket : sockets_) {
            check_fd(socket.get());
            FD_SET(socket.get(), &reading);
            highest = std::max(highest, socket.get());
        }
        if (writing >= 0) FD_SET(sockets_[writing].get(), &writable);
        timeval timeout{};
        int result = select(static_cast<int>(highest + 1), &reading, writing >= 0 ? &writable : nullptr,
                            nullptr, deadline.remaining(timeout));
        if (result < 0) {
            if (retryable(socket_error())) return false;
            throw std::runtime_error("Socket polling failed: " + std::to_string(socket_error()));
        }
        if (result == 0) throw std::runtime_error("Session timed out");
        for (std::size_t player = 0; player < sockets_.size(); ++player) {
            if (!FD_ISSET(sockets_[player].get(), &reading)) continue;
            std::array<std::uint8_t, 4096> incoming{};
            const std::size_t available = incoming.size() - queues_[player].size();
            if (available == 0) throw std::runtime_error("Player sent too much data ahead of the protocol");
            int received = recv(sockets_[player].get(), reinterpret_cast<char*>(incoming.data()), static_cast<int>(available), 0);
            if (received == 0) throw Disconnected(player);
            if (received < 0) {
                if (retryable(socket_error())) continue;
                throw Disconnected(player);
            }
            queues_[player].insert(queues_[player].end(), incoming.begin(), incoming.begin() + received);
            if (queues_[player].front() == 'S' && queues_[player].size() >= (player == 0 ? 8u : 4u))
                throw StopRequested{player};
        }
        return writing >= 0 && FD_ISSET(sockets_[writing].get(), &writable);
    }
    std::vector<Socket>& sockets_;
    std::vector<std::deque<std::uint8_t>> queues_;
    int timeout_ms_;
};

// Shutdown notifications are bounded and do not attempt to read a dead peer.
static void notify_stop(const std::vector<Socket>& sockets, std::size_t skip) noexcept {
    for (std::size_t player = 0; player < sockets.size(); ++player) {
        if (player == skip) continue;
        Bytes packet((player == 0 ? sockets.size() - 1 : sockets.size()) * 4, 0);
        packet[0] = 'S';
        std::size_t sent = 0;
        try {
            Deadline deadline(500);
            while (sent < packet.size()) {
                fd_set writable;
                FD_ZERO(&writable);
                FD_SET(sockets[player].get(), &writable);
                timeval timeout{};
                int ready = select(static_cast<int>(sockets[player].get() + 1), nullptr, &writable, nullptr, deadline.remaining(timeout));
                if (ready <= 0) break;
                int count = send_bytes(sockets[player].get(), packet.data() + sent, packet.size() - sent);
                if (count > 0) sent += static_cast<std::size_t>(count);
                else if (count == 0 || !retryable(socket_error())) break;
            }
        } catch (...) {}
    }
}

static Socket listen_on(const std::string& address, int port, int& actual_port) {
    Socket listener(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));
    if (listener.get() == invalid_socket) throw std::runtime_error("Cannot create listening socket");
    check_fd(listener.get());
    sockaddr_in endpoint{};
    endpoint.sin_family = AF_INET;
    endpoint.sin_port = htons(static_cast<std::uint16_t>(port));
    if (inet_pton(AF_INET, address.c_str(), &endpoint.sin_addr) != 1)
        throw std::runtime_error("--bind requires an IPv4 address");
    int enabled = 1;
#ifdef _WIN32
    setsockopt(listener.get(), SOL_SOCKET, SO_EXCLUSIVEADDRUSE, reinterpret_cast<const char*>(&enabled), sizeof(enabled));
#else
    setsockopt(listener.get(), SOL_SOCKET, SO_REUSEADDR, &enabled, sizeof(enabled));
#endif
    if (bind(listener.get(), reinterpret_cast<sockaddr*>(&endpoint), sizeof(endpoint)) != 0)
        throw std::runtime_error("Cannot bind port " + std::to_string(port) + ": " + std::to_string(socket_error()));
    if (listen(listener.get(), 4) != 0) throw std::runtime_error("Cannot listen on socket");
#ifdef _WIN32
    int length = sizeof(endpoint);
#else
    socklen_t length = sizeof(endpoint);
#endif
    if (getsockname(listener.get(), reinterpret_cast<sockaddr*>(&endpoint), &length) != 0)
        throw std::runtime_error("Cannot query listening port");
    actual_port = ntohs(endpoint.sin_port);
    nonblocking(listener.get());
    return listener;
}

static std::vector<Socket> accept_players(SocketHandle listener, int players, int timeout_ms) {
    std::vector<Socket> clients;
    clients.reserve(players);
    Deadline deadline(timeout_ms);
    while (static_cast<int>(clients.size()) < players) {
        fd_set reading;
        FD_ZERO(&reading);
        FD_SET(listener, &reading);
        SocketHandle highest = listener;
        for (const auto& client : clients) {
            FD_SET(client.get(), &reading);
            highest = std::max(highest, client.get());
        }
        timeval timeout{};
        int ready = select(static_cast<int>(highest + 1), &reading, nullptr, nullptr, deadline.remaining(timeout));
        if (ready < 0) {
            if (retryable(socket_error())) continue;
            throw std::runtime_error("Failed while waiting for players");
        }
        if (ready == 0) throw std::runtime_error("Timed out waiting for players");
        for (std::size_t i = 0; i < clients.size(); ++i) {
            if (FD_ISSET(clients[i].get(), &reading)) {
                char byte;
                int received = recv(clients[i].get(), &byte, 1, MSG_PEEK);
                if (received == 0) throw Disconnected(i);
                if (received > 0) throw std::runtime_error("Client sent data before the player-ID handshake");
                if (!retryable(socket_error())) throw Disconnected(i);
            }
        }
        if (!FD_ISSET(listener, &reading)) continue;
        Socket client(accept(listener, nullptr, nullptr));
        if (client.get() == invalid_socket) {
            if (retryable(socket_error())) continue;
            throw std::runtime_error("Cannot accept player");
        }
        check_fd(client.get());
        int enabled = 1;
        if (setsockopt(client.get(), IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<const char*>(&enabled), sizeof(enabled)) != 0)
            throw std::runtime_error("Cannot configure TCP_NODELAY");
        nonblocking(client.get());
        clients.push_back(std::move(client));
        std::cout << "CONNECTED " << clients.size() << '/' << players << std::endl;
    }
    return clients;
}

static int session(std::vector<Socket>& clients, int timeout_ms) {
    Transport transport(clients, timeout_ms);
    try {
        for (std::size_t player = 0; player < clients.size(); ++player)
            transport.write_exact(player, Bytes{static_cast<std::uint8_t>(player), 0, 0, 0});
        std::cout << "READY " << clients.size() << std::endl;
        std::vector<Bytes> previous(clients.size(), Bytes(4, 0));
        for (std::size_t i = 1; i < clients.size(); ++i)
            previous[i] = Bytes{'M', static_cast<std::uint8_t>(0x03u | (i << 6)), 0xff, 0xff};
        for (;;) {
            Bytes host = transport.read_exact(0, 8);
            for (std::size_t player = 1; player < clients.size(); ++player) {
                Bytes outgoing = host;
                if (clients.size() == 4 && player == 3) {
                    outgoing.insert(outgoing.end(), previous[2].begin(), previous[2].end());
                    outgoing.insert(outgoing.end(), previous[1].begin(), previous[1].end());
                } else {
                    for (std::size_t other = 1; other < clients.size(); ++other)
                        if (other != player) outgoing.insert(outgoing.end(), previous[other].begin(), previous[other].end());
                }
                transport.write_exact(player, outgoing);
            }
            Bytes replies;
            for (std::size_t player = 1; player < clients.size(); ++player) {
                previous[player] = transport.read_exact(player, 4);
                replies.insert(replies.end(), previous[player].begin(), previous[player].end());
            }
            transport.write_exact(0, replies);
        }
    } catch (const StopRequested& stop) {
        notify_stop(clients, stop.player);
        std::cout << "STOP player=" << stop.player + 1 << std::endl;
        return 0;
    } catch (const Disconnected& error) {
        notify_stop(clients, error.player);
        std::cerr << error.what() << std::endl;
        return 1;
    } catch (const std::exception& error) {
        notify_stop(clients, clients.size());
        std::cerr << error.what() << std::endl;
        return 1;
    }
}

static int number(const std::string& value, int minimum, int maximum) {
    if (value.empty() || value.find_first_not_of("0123456789") != std::string::npos)
        throw std::runtime_error("Invalid number: " + value);
    unsigned long parsed;
    try { parsed = std::stoul(value); }
    catch (...) { throw std::runtime_error("Number outside allowed range"); }
    if (parsed < static_cast<unsigned long>(minimum) || parsed > static_cast<unsigned long>(maximum))
        throw std::runtime_error("Number outside allowed range: " + value);
    return static_cast<int>(parsed);
}
int main(int argc, char** argv) {
    try {
        int players = 0, port = -1, timeout_ms = 0;
        std::string address = "127.0.0.1";
        for (int i = 1; i < argc; ++i) {
            std::string option = argv[i];
            if (option == "--help") {
                std::cout << "VBALink 1.72 LAN relay (development build)\n"
                             "--players 2|3|4  --bind IPv4  --port 0..65535  --timeout-ms 0..3600000\n"
                             "Defaults: local computer only; legacy port 6478/6479/6480; no idle timeout.\n"
                             "For LAN: --bind 0.0.0.0. --port 0 chooses a test port.\n";
                return 0;
            }
            if (i + 1 == argc) throw std::runtime_error("Missing value for " + option);
            std::string value = argv[++i];
            if (option == "--players") players = number(value, 2, 4);
            else if (option == "--bind") address = value;
            else if (option == "--port") port = number(value, 0, 65535);
            else if (option == "--timeout-ms") timeout_ms = number(value, 0, 3600000);
            else throw std::runtime_error("Unknown option: " + option);
        }
        if (!players) {
            std::cout << "Select number of players (2-4): " << std::flush;
            std::string input;
            if (!std::getline(std::cin, input)) throw std::runtime_error("No player count provided");
            players = number(input, 2, 4);
        }
        if (port < 0) port = 6476 + players;
        NetworkRuntime runtime;
        int actual_port = 0;
        Socket listener = listen_on(address, port, actual_port);
        std::cout << "LISTENING " << address << ' ' << actual_port << std::endl;
        auto clients = accept_players(listener.get(), players, timeout_ms);
        return session(clients, timeout_ms);
    } catch (const std::exception& error) {
        std::cerr << error.what() << std::endl;
        return 1;
    }
}
