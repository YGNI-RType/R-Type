#include "socket.hpp"
#include <stdexcept>

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

#define NET_MAX_PAYLOAD 4000
#define NET_MAX_MESSAGE 4096
#define MIN_ROUTEABLE_PACKET 16
#define MAX_ROUTEABLE_PACKET 1400 // Ethernet 1518 - ( CRC + IP + UDP header)
#define UDP_HEADER_SIZE 28

int ASocket::socketClose(void) {
    int status = 0;

#ifdef _WIN32
    status = shutdown(m_sock, SD_BOTH);
    if (status == 0)
        status = closesocket(m_sock);
#else
    status = shutdown(m_sock, SHUT_RDWR);
    if (status == 0)
        status = close(m_sock);
#endif

    return status;
}

void ASocket::setBlocking(bool blocking) {
#ifdef _WIN32
    u_long mode = blocking ? 0 : 1;
    ioctlsocket(m_sock, FIONBIO, &mode);
#else
    int flags = fcntl(m_sock, F_GETFL, 0);
    if (blocking)
        flags &= ~O_NONBLOCK;
    else
        flags |= O_NONBLOCK;
    fcntl(m_sock, F_SETFL, flags);
#endif
}

ASocket::~ASocket() { socketClose(); }

/***********************************************/

SocketTCP::SocketTCP() {
    m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_sock == -1)
        throw std::runtime_error("Failed to create socket");
}

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

SocketUDP::SocketUDP(const std::string &address) {
    m_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (m_sock == -1)
        throw std::runtime_error("Failed to create socket");

    unsigned int opt = 1;
    setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));

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
        auto sent = sendto(m_sock, data, size - sentTotal, 0,
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
        recvfrom(m_sock, data, size, 0, (struct sockaddr *)&m_addr, &len);
}

} // namespace Network