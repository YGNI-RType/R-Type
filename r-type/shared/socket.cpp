#include "socket.hpp"
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

#define MAX_PACKETLEN 1400 // max size of a network packet

#define FRAGMENT_SIZE (MAX_PACKETLEN - 100)
#define PACKET_HEADER 10 // two ints and a short

#define UDP_SO_RCVBUF_SIZE 131072

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

SocketTCP::SocketTCP() {
    m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_sock == -1)
        throw std::runtime_error("Failed to create socket");
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

int SocketUDP::initSocket(void) {
    mg_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (mg_sock == -1)
        return -1;

    unsigned int opt = 1;
    setsockopt(mg_sock, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));
    return 0;
}

int SocketUDP::shutdownSocket(void) {
    return socketClose(mg_sock);
}

SocketUDP::SocketUDP(const std::string &address) {

    std::memcpy(&m_addr, 0, sizeof(m_addr));
    if (address == "localhost")
        m_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    else
        m_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // setsockopt(m_sock, SOL_SOCKET, SO_RCVBUF, (char *)&opt, sizeof(opt));
    // setsockopt(m_sock, SOL_SOCKET, SO_SNDBUF, (char *)&opt, sizeof(opt));
}

size_t SocketUDP::send(const byte_t *data, const size_t size) const {
    size_t sentTotal = 0;

    /*
        BY THE WAY, m_sock is the same for EVERY CLASS, i don't put it on static
        because it's inherited
    */
    while (sentTotal < size) {
        auto sent = sendto(mg_sock, data, size - sentTotal, 0,
                           (struct sockaddr *)&m_addr, sizeof(m_addr));
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
    socklen_t len = sizeof(m_addr);
    size_t recv =
        recvfrom(mg_sock, data, size, 0, (struct sockaddr *)&m_addr, &len);
}

} // namespace Network