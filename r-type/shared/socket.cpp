/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet (Workspace)
** File description:
** socket
*/

#include "shared/socketError.hpp"
#include "shared/socket.hpp"

#include <cerrno>
#include <cstring>
#include <stdexcept>
#include <string>

#ifdef _WIN32
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501 /* Windows XP. */
#endif
#include <Ws2tcpip.h>
#include <winsock2.h>
#else
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

namespace Network {

#ifdef _WIN32
WSADATA ASocket::winsockdata;
#endif

void ASocket::initLibs(void) {
#ifdef _WIN32
    if (WSAStartup(MAKEWORD(1, 1), &winsockdata)) {
        // Com_Printf( "WARNING: Winsock initialization failed, returned %d\n",
        // r );
        return;
    }

    // Com_Printf( "Winsock Initialized\n" );
#endif
}

int ASocket::socketClose(const SOCKET socket) {
    int status = 0;

#ifdef _WIN32
    status = shutdown(m_sock, SD_BOTH);
    if (status == 0)
        status = closesocket(m_sock);
#else
    status = shutdown(socket, SHUT_RDWR);
    if (status == 0)
        status = close(socket);
#endif

    return status;
}

void ASocket::setBlocking(const SOCKET socket, bool blocking) {
#ifdef _WIN32
    u_long mode = blocking ? 0 : 1;
    ioctlsocket(m_sock, FIONBIO, &mode);
#else
    int flags = fcntl(socket, F_GETFL, 0);
    if (blocking)
        flags &= ~O_NONBLOCK;
    else
        flags |= O_NONBLOCK;
    fcntl(socket, F_SETFL, flags);
#endif
}

/***********************************************/

ASocket::SOCKET SocketTCPMaster::mg_sock = -1;

SocketTCPMaster::SocketTCPMaster(const IP &ip, uint16_t port) : ip(ip) {
    mg_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_TCP);
    if (mg_sock == -1)
        throw std::runtime_error("(TCP) Failed to create socket");

    unsigned int opt = 1;
    if (setsockopt(mg_sock, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
                   sizeof(opt)))
        throw std::runtime_error("(TCP) Failed to set socket options (SO_REUSEADDR)");
    if (setsockopt(mg_sock, SOL_SOCKET, SO_BROADCAST, (char *)&opt,
                   sizeof(opt)))
        throw std::runtime_error("(TCP) Failed to set socket options (SO_BROADCAST)");

    struct sockaddr_in address;
    std::memcpy(&address, &ip.addr, sizeof(address));

    address.sin_port = port == PORT_ANY ? 0 : htons(port);

    if (bind(mg_sock, (sockaddr *)&address, sizeof(address)) < 0)
        throw SocketException("(TCP) Failed to bind socket");

    if (listen(mg_sock, MAX_LISTEN) < 0)
        throw SocketException("(TCP) Failed to listen on socket");
}

SocketTCP SocketTCPMaster::accept(void) const { return SocketTCP(*this); }

SocketTCPMaster::~SocketTCPMaster() { socketClose(mg_sock); }

/***********************************************/

SocketTCP::SocketTCP(const SocketTCPMaster &socketMaster) {
    // addr should not be null
    m_sock = accept(socketMaster.getSocket(), nullptr, nullptr);
    if (m_sock == -1) {
        if (errno != EWOULDBLOCK)
            throw std::runtime_error("Failed to accept connection");
    }
}

SocketTCP::~SocketTCP() { socketClose(m_sock); }

size_t SocketTCP::send(const byte_t *data, const size_t size) const {
    size_t sentTotal = 0;

    while (sentTotal < size) {
        auto sent = ::send(m_sock, data, size - sentTotal, 0);
        if (sent < 0) {
            if (errno == EWOULDBLOCK)
                return sentTotal;
            return sent;
        }
        if (sent == 0)
            return 0;
        sentTotal += sent;
    }
    return sentTotal;
}

size_t SocketTCP::receive(byte_t *data, const size_t size) const {
    size_t receivedTotal = 0;

    while (receivedTotal < size) {
        auto received = recv(m_sock, data, size - receivedTotal, 0);
        if (received < 0) {
            if (errno == EWOULDBLOCK)
                return receivedTotal;
            return received;
        }
        if (received == 0)
            return 0;
        receivedTotal += received;
    }
    return receivedTotal;
}

/***********************************************/

ASocket::SOCKET SocketUDP::mg_sock = -1;

SocketUDP::~SocketUDP() { socketClose(mg_sock); }

SocketUDP::SocketUDP(const IP &ip, uint16_t port) : ip(ip) {

    mg_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (mg_sock == -1)
        throw std::runtime_error("(UDP) Failed to create socket");

    unsigned int opt = 1;
    if (setsockopt(mg_sock, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
                   sizeof(opt)))
        throw std::runtime_error("(UDP) Failed to set socket options (SO_REUSEADDR)");
    if (setsockopt(mg_sock, SOL_SOCKET, SO_BROADCAST, (char *)&opt,
                   sizeof(opt)))
        throw std::runtime_error("(UDP) Failed to set socket options (SO_BROADCAST)");

    struct sockaddr_in address;
    std::memcpy(&address, &ip.addr, sizeof(address));

    address.sin_port = port == PORT_ANY ? 0 : htons(port);

    if (bind(mg_sock, (sockaddr *)&address, sizeof(address)) < 0)
        throw SocketException("(UDP) Failed to bind socket");
}

size_t SocketUDP::send(const byte_t *data, const size_t size) const {
    size_t sentTotal = 0;

    /*
        BY THE WAY, m_sock is the same for EVERY CLASS, i don't put it on static
        because it's inherited
    */
    while (sentTotal < size) {
        auto sent = sendto(mg_sock, data, size - sentTotal, 0,
                           (struct sockaddr *)&ip.addr, sizeof(struct sockaddr));
        if (sent < 0) {
            if (errno == EWOULDBLOCK)
                return sentTotal;
            return sent;
        }
        if (sent == 0)
            return 0;
        sentTotal += sent;
    }
    return sentTotal;
}

size_t SocketUDP::receive(byte_t *data, const size_t size) const {
    socklen_t len = sizeof(ip.addr);
    size_t recv =
        recvfrom(mg_sock, data, size, 0, (struct sockaddr *)&ip.addr, &len);

    return recv;
}

} // namespace Network