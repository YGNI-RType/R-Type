/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet (Workspace)
** File description:
** socket
*/

#include "GEngine/net/socket.hpp"
#include "GEngine/net/socketError.hpp"

#include <algorithm>
#include <cerrno>
#include <cstring>
#include <stdexcept>
#include <string>

#ifdef _WIN32
#include <WS2tcpip.h>
#include <winsock2.h>
#include <ws2spi.h>
#include <ws2tcpip.h>
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

SOCKET ASocket::m_highFd = -1;
fd_set ASocket::m_fdSet;

ASocket::~ASocket() { socketClose(); }

ASocket::ASocket(ASocket &&other) {
    m_sock = other.m_sock;
    m_port = other.m_port;
    other.m_sock = -1;
}

ASocket &ASocket::operator=(ASocket &&other) {
    if (this != &other) {
        m_sock = other.m_sock;
        m_port = other.m_port;
        other.m_sock = -1;
    }
    return *this;
}

//////////////////////////////////////

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

int ASocket::socketClose(void) {
    int status = 0;

    if (m_sock == -1)
        return 0;

#ifdef _WIN32
    status = shutdown(m_sock, SD_BOTH);
    if (status == 0)
        status = closesocket(m_sock);
#else
    status = shutdown(m_sock, SHUT_RDWR);
    if (status == 0)
        status = close(m_sock);
#endif

    if (m_sock == m_highFd)
        while (!FD_ISSET(m_highFd, &m_fdSet))
            (m_highFd)--;

    return status;
}

void ASocket::addSocketPool(SOCKET socket) {
    FD_SET(socket, &m_fdSet);
    if (socket > m_highFd)
        m_highFd = socket;
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

bool ASocket::isBlocking(void) const {
#ifdef _WIN32
    u_long mode;
    ioctlsocket(m_sock, FIONBIO, &mode);
    return mode == 0;
#else
    int flags = fcntl(m_sock, F_GETFL, 0);
    return (flags & O_NONBLOCK) == 0;
#endif
}

/***********************************************/

SocketTCPMaster::SocketTCPMaster(const IP &ip, uint16_t port) {
    m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_sock == -1)
        throw std::runtime_error("(TCP) Failed to create socket");

    m_port = port;
    struct sockaddr_in address;
    std::memcpy(&address, &ip.addr, sizeof(struct sockaddr_in));

    address.sin_port = port == PORT_ANY ? 0 : htons(port);

    if (bind(m_sock, (sockaddr *)&address, sizeof(address)) < 0)
        throw SocketException("(TCP) Failed to bind socket");

    if (listen(m_sock, MAX_LISTEN) < 0)
        throw SocketException("(TCP) Failed to listen on socket");

    addSocketPool(m_sock);
}

SocketTCPMaster::SocketTCPMaster(SocketTCPMaster &&other) : ASocket(std::move(other)) {}
SocketTCPMaster &SocketTCPMaster::operator=(SocketTCPMaster &&other) {
    if (this != &other)
        ASocket::operator=(std::move(other));
    return *this;
}

SocketTCP SocketTCPMaster::accept(UnknownAddress &unkwAddr) const { return SocketTCP(*this, unkwAddr); }

/***********************************************/

SocketTCP::SocketTCP(const SocketTCPMaster &socketMaster, UnknownAddress &unkwAddr) {
    m_sock = accept(socketMaster.getSocket(), unkwAddr.getAddr(), &unkwAddr.getLen());
    if (m_sock < 0) {
        // if (errno != EWOULDBLOCK) /* it should never block right ? */
        throw std::runtime_error("Failed to accept connection");
    }

    m_port = socketMaster.getPort();
    addSocketPool(m_sock);
}

SocketTCP::SocketTCP(const Address &addr, uint16_t port) {
    m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_sock == -1)
        throw std::runtime_error("(TCP) Failed to create socket");

    m_port = port;
    struct sockaddr_in address;
    std::memcpy(&address, &addr, sizeof(struct sockaddr_in));

    address.sin_port = port == PORT_ANY ? 0 : htons(port);

    if (connect(m_sock, (sockaddr *)&address, sizeof(address)) < 0) {
        if (errno == EINPROGRESS)
            return;
        socketClose();
        throw SocketException("(TCP) Failed to connect to server");
    }

    addSocketPool(m_sock);
}

SocketTCP::SocketTCP(SocketTCP &&other) : ASocket(std::move(other)) {}
SocketTCP &SocketTCP::operator=(SocketTCP &&other) {
    if (this != &other)
        ASocket::operator=(std::move(other));
    return *this;
}

bool SocketTCP::send(const TCPMessage &msg) const {
    /* TODO : here, we guess the size is enough to
    send, should we break it here or before ? */

    TCPSerializedMessage sMsg;
    msg.getSerialize(sMsg);

    return sendReliant(&sMsg, msg.getSize()) != 0; // if it did not block
}

void SocketTCP::receive(TCPMessage &msg) const {
    TCPSerializedMessage sMsg;

    size_t recvSz = receiveReliant(&sMsg, sizeof(HeaderSerializedMessage));
    /* WIN : need to use these parenthesis, to skip windows.h macro */
    recvSz = receiveReliant(&sMsg + recvSz, (std::min)(sMsg.curSize, sizeof(TCPSerializedMessage) - recvSz));

    msg.setSerialize(sMsg);
}

size_t SocketTCP::receiveReliant(TCPSerializedMessage *buffer, size_t size) const {
    size_t receivedTotal = 0;

    while (receivedTotal < size) {
        auto received = ::recv(m_sock, reinterpret_cast<char *>(buffer + receivedTotal), size - receivedTotal, 0);
        if (received < 0)
            throw SocketException("Failed to receive message");
        if (received == 0)
            throw SocketClientDisconnected();
        receivedTotal += received;
    }
    return receivedTotal;
}

/* THIS IS THE ALL MESSAGE, NOT THE JUST DATA */
size_t SocketTCP::sendReliant(const TCPSerializedMessage *msg, size_t msgDataSize) const {
    size_t sentTotal = 0;
    msgDataSize += sizeof(TCPMessage) - MAX_TCP_MSGLEN;

    while (sentTotal < msgDataSize) {
        auto sent = ::send(m_sock, reinterpret_cast<const char *>(msg + sentTotal), msgDataSize - sentTotal, 0);
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

SocketUDP::SocketUDP(const IP &ip, uint16_t port) {

    m_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (m_sock == -1)
        throw std::runtime_error("(UDP) Failed to create socket");

    unsigned int opt = 1;
    if (setsockopt(m_sock, SOL_SOCKET, SO_BROADCAST, (char *)&opt, sizeof(opt)))
        throw std::runtime_error("(UDP) Failed to set socket options (SO_BROADCAST)");

    struct sockaddr_in address;
    std::memcpy(&address, &ip.addr, sizeof(address));

    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = port == PORT_ANY ? 0 : htons(port);

    if (bind(m_sock, (sockaddr *)&address, sizeof(address)) < 0)
        throw SocketException("(UDP) Failed to bind socket");

    addSocketPool(m_sock);
}

SocketUDP::SocketUDP(SocketUDP &&other) : ASocket(std::move(other)) {}
SocketUDP &SocketUDP::operator=(SocketUDP &&other) {
    if (this != &other)
        ASocket::operator=(std::move(other));
    return *this;
}

/* TODO : sending some unitialized bytes by default */
bool SocketUDP::send(const UDPMessage &msg, const Address &addr) const {
    /*
        BY THE WAY, m_sock is the same for EVERY CLASS, i don't put it on static
        because it's inherited
    */
    UDPSerializedMessage sMsg;
    size_t size = msg.getSize();
    struct sockaddr_storage sockaddr = {0};
    msg.getSerialize(sMsg);

    addr.toSockAddr(reinterpret_cast<struct sockaddr *>(&sockaddr));
    auto sent =
        sendto(m_sock, reinterpret_cast<const char *>(&sMsg), size + sizeof(UDPSerializedMessage) - MAX_UDP_MSGLEN, 0,
               (struct sockaddr *)&sockaddr, sizeof(struct sockaddr));
    if (sent < 0) {
        if (errno == EWOULDBLOCK || errno == EAGAIN)
            return false;
        throw SocketException("Failed to send message (invalid address)");
    }
    return true;
}

void SocketUDP::receive(struct sockaddr *addr, UDPSerializedMessage &data, socklen_t *len) const {
    size_t recv = recvfrom(m_sock, reinterpret_cast<char *>(&data), sizeof(UDPSerializedMessage), 0, addr, len);

    // checking wouldblock etc... select told us to read so it's not possible
    if (recv < 0)
        throw SocketException("Failed to receive message");

    if (recv < sizeof(UDPSerializedMessage) - MAX_UDP_MSGLEN)
        throw SocketException("Received message is too small");
}

AddressV4 SocketUDP::receiveV4(UDPMessage &msg) const {
    UDPSerializedMessage sMsg;
    struct sockaddr_in addr = {0};
    socklen_t len = sizeof(addr);

    receive(reinterpret_cast<struct sockaddr *>(&addr), sMsg, &len);
    msg.setSerialize(sMsg);

    return AddressV4(AT_IPV4, ntohs(addr.sin_port), ntohl(addr.sin_addr.s_addr));
}

AddressV6 SocketUDP::receiveV6(UDPMessage &msg) const {
    UDPSerializedMessage sMsg;
    struct sockaddr_in6 addr;
    socklen_t len = sizeof(addr);

    receive(reinterpret_cast<struct sockaddr *>(&addr), sMsg, &len);
    msg.setSerialize(sMsg);

    return AddressV6(AT_IPV6, ntohs(addr.sin6_port), addr.sin6_addr, addr.sin6_scope_id);
}

/*****************************************************/

SocketTCPMaster openSocketTcp(const IP &ip, uint16_t wantedPort) {
    for (uint16_t i = 0; i < MAX_TRY_PORTS; i++) {
        try {
            return std::move(SocketTCPMaster(ip, wantedPort + i));
        } catch (SocketException &e) {
            if (!e.shouldRetry() || i == MAX_TRY_PORTS - 1)
                throw e;
            continue;
        }
    }
    throw SocketException("Failed to open TCP socket");
}

SocketUDP openSocketUdp(const IP &ip, uint16_t wantedPort) {
    for (uint16_t i = 0; i < MAX_TRY_PORTS; i++) {
        try {
            return std::move(SocketUDP(ip, wantedPort + i));
        } catch (SocketException &e) {
            if (!e.shouldRetry() || i == MAX_TRY_PORTS - 1)
                throw e;
            continue;
        }
    }
    throw SocketException("Failed to open UDP socket");
}

} // namespace Network