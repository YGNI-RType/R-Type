/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet (Workspace)
** File description:
** socket
*/

#include "GEngine/net/socket.hpp"
#include "GEngine/net/socketError.hpp"

#include <cerrno>
#include <cstring>
#include <stdexcept>
#include <string>

#ifdef _WIN32
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501 /* Windows XP. */
#endif
#include <Ws2tcpiph>
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

ASocket::SOCKET ASocket::m_highFd = -1;
fd_set ASocket::m_fdSet;

void ASocket::initLibs(void) {
#ifdef _WIN32
    if (WSAStartup(MAKEWORD(1, 1), &winsockdata)) {
        // Com_Printf( "WARNING: Winsock initialization failed, returned %d\n",
        // r );
        return;
    }

    // Com_Printf( "Winsock Initialized\n" );
#endif
    FD_ZERO(&m_fdSet);
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

    if (socket == m_highFd)
        while (!FD_ISSET(m_highFd, &m_fdSet))
            (m_highFd)--;

    return status;
}

void ASocket::addSocketPool(SOCKET socket) {
    FD_SET(socket, &m_fdSet);
    if (socket > m_highFd)
        m_highFd = socket;
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

SocketTCPMaster::SocketTCPMaster(const IP &ip, uint16_t port) {
    mg_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_TCP);
    if (mg_sock == -1)
        throw std::runtime_error("(TCP) Failed to create socket");

    unsigned int opt = 1;
    if (setsockopt(mg_sock, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
                   sizeof(opt)))
        throw std::runtime_error(
            "(TCP) Failed to set socket options (SO_REUSEADDR)");

    struct sockaddr_in address;
    std::memcpy(&address, &ip.addr, sizeof(address));

    address.sin_port = port == PORT_ANY ? 0 : htons(port);

    if (bind(mg_sock, (sockaddr *)&address, sizeof(address)) < 0)
        throw SocketException("(TCP) Failed to bind socket");

    if (listen(mg_sock, MAX_LISTEN) < 0)
        throw SocketException("(TCP) Failed to listen on socket");

    addSocketPool(mg_sock);
}

SocketTCP SocketTCPMaster::accept(size_t pos) const {
    return SocketTCP(pos, *this);
}

SocketTCPMaster::~SocketTCPMaster() { socketClose(mg_sock); }

/***********************************************/

SocketTCP::SocketTCP(size_t pos_accept, const SocketTCPMaster &socketMaster)
    : m_posAccept(m_posAccept) {

    m_sock = accept(socketMaster.getSocket(), &m_addr, &m_szAddr);
    if (m_sock < 0) {
        if (errno != EWOULDBLOCK) /* it should never block right ? */
            throw std::runtime_error("Failed to accept connection");
    }

    addSocketPool(m_sock);
}

SocketTCP::~SocketTCP() { socketClose(m_sock); }

bool SocketTCP::send(const TCPMessage &msg) const {
    /* TODO : here, we guess the size is enough to
    send, should we break it here or before ? */

    return sendReliant(&msg, msg.getSize()) != 0; // if it did not block
}

TCPMessage SocketTCP::receive(void) const {
    byte_t data[sizeof(TCPMessage)];

    size_t recvSz = receiveReliant(data, sizeof(AMessage));
    TCPMessage *msg = reinterpret_cast<TCPMessage *>(data);
    recvSz = receiveReliant(
        data + recvSz, std::min(msg->getSize(), sizeof(TCPMessage) - recvSz));

    return *msg;
}

size_t SocketTCP::receiveReliant(byte_t *buffer, size_t size) const {
    size_t receivedTotal = 0;

    while (receivedTotal < size) {
        auto received =
            ::recv(m_sock, buffer + receivedTotal, size - receivedTotal, 0);
        if (received < 0)
            throw SocketException("Failed to receive message");
        if (received == 0)
            throw SocketClientDisconnected();
        receivedTotal += received;
    }
    return receivedTotal;
}

/* THIS IS THE ALL MESSAGE, NOT THE DATA */
size_t SocketTCP::sendReliant(const TCPMessage *msg, size_t msgDataSize) const {
    size_t sentTotal = 0;
    msgDataSize += sizeof(TCPMessage) - MAX_TCP_MSGLEN;

    while (sentTotal < msgDataSize) {
        auto sent = ::send(m_sock, msg + sentTotal, msgDataSize - sentTotal, 0);
        if (sent < 0) {
            if (errno == EWOULDBLOCK || errno == EAGAIN)
                return 0;
            throw SocketException("Failed to send message");
        }
        if (sent == 0)
            throw SocketClientDisconnected();
        sentTotal += sent;
    }
    return sentTotal;
}

/***********************************************/

ASocket::SOCKET SocketUDP::mg_sock = -1;

SocketUDP::~SocketUDP() { socketClose(mg_sock); }

SocketUDP::SocketUDP(const IP &ip, uint16_t port) {

    mg_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (mg_sock == -1)
        throw std::runtime_error("(UDP) Failed to create socket");

    unsigned int opt = 1;
    if (setsockopt(mg_sock, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
                   sizeof(opt)))
        throw std::runtime_error(
            "(UDP) Failed to set socket options (SO_REUSEADDR)");
    if (setsockopt(mg_sock, SOL_SOCKET, SO_BROADCAST, (char *)&opt,
                   sizeof(opt)))
        throw std::runtime_error(
            "(UDP) Failed to set socket options (SO_BROADCAST)");

    struct sockaddr_in address;
    std::memcpy(&address, &ip.addr, sizeof(address));

    address.sin_port = port == PORT_ANY ? 0 : htons(port);

    if (bind(mg_sock, (sockaddr *)&address, sizeof(address)) < 0)
        throw SocketException("(UDP) Failed to bind socket");

    addSocketPool(mg_sock);
}

bool SocketUDP::send(const UDPMessage &msg, const Address &addr) const {

    /*
        BY THE WAY, m_sock is the same for EVERY CLASS, i don't put it on static
        because it's inherited
    */
    size_t size = msg.getSize();
    struct sockaddr_storage sockaddr = {0};

    addr.toSockAddr(reinterpret_cast<struct sockaddr *>(&sockaddr));
    auto sent =
        sendto(mg_sock, &msg, size + sizeof(UDPMessage) - MAX_UDP_MSGLEN, 0,
               (struct sockaddr *)&sockaddr, sizeof(struct sockaddr));
    if (sent < 0) {
        if (errno == EWOULDBLOCK || errno == EAGAIN)
            return false;
        throw SocketException("Failed to send message");
    }
    if (sent == 0)
        throw SocketClientDisconnected();
    return true;
}

void SocketUDP::receive(struct sockaddr *addr, byte_t *data) const {
    socklen_t len;

    size_t recv = recvfrom(mg_sock, data, sizeof(UDPMessage), 0, addr, &len);

    // checking wouldblock etc... select told us to read so it's not possible
    if (recv < 0)
        throw SocketException("Failed to receive message");

    if (recv < sizeof(UDPMessage) - MAX_UDP_MSGLEN)
        throw SocketException("Received message is too small");
}

std::pair<UDPMessage, AddressV4> SocketUDP::receiveV4(void) const {
    byte_t data[sizeof(UDPMessage)];
    struct sockaddr_in addr;

    receive(reinterpret_cast<struct sockaddr *>(&addr), data);
    UDPMessage *msg = reinterpret_cast<UDPMessage *>(data);

    AddressV4 a(AT_IPV4, addr.sin_port, addr.sin_addr.s_addr);
    return std::make_pair(*msg, a);
}

std::pair<UDPMessage, AddressV6> SocketUDP::receiveV6(void) const {
    byte_t data[sizeof(UDPMessage)];
    struct sockaddr_in6 addr;

    receive(reinterpret_cast<struct sockaddr *>(&addr), data);
    UDPMessage *msg = reinterpret_cast<UDPMessage *>(data);

    AddressV6 a(AT_IPV6, addr.sin6_port, addr.sin6_addr, addr.sin6_scope_id);
    return std::make_pair(*msg, a);
}

} // namespace Network