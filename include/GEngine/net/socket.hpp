/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet (Workspace)
** File description:
** socket
*/

#pragma once

#include "msg.hpp"
#include "net_address.hpp"
#include "net_common.hpp"

#include <cstdint>
#include <utility>
#include <string>

#ifndef _WIN32
#include <netinet/in.h> // Add this line to include the header file that defines sockaddr_in
#include <sys/socket.h>
#include <sys/types.h>
#endif

#ifndef _WIN32
  typedef int SOCKET;
#endif

namespace Network {

#define MAX_PACKETLEN 1400 // max size of a network packet

#define FRAGMENT_SIZE (MAX_PACKETLEN - 100)
#define PACKET_HEADER 10 // two ints and a short

#define UDP_SO_RCVBUF_SIZE 131072

////////////////////////////////////////

class ASocket {

  public:
#ifdef _WIN32
    static WSADATA winsockdata;
#endif

    static SOCKET getHighestSocket(void) { return m_highFd; }

    SOCKET getSocket(void) const { return m_sock; }

  public:
    static void initLibs(void);

    ASocket() = default;
    ASocket(const ASocket &other) = delete;
    ASocket &operator=(const ASocket &other) = delete;
    ASocket(ASocket &&other);
    ASocket &operator=(ASocket &&other);

  protected:
    static void addSocketPool(SOCKET socket);
    virtual ~ASocket();

  private:
    static fd_set m_fdSet;
    static SOCKET m_highFd;

  public:
    void setBlocking(bool blocking);
  protected:
    int socketClose(void);

    SOCKET m_sock = -1;
};

////////////////////////////////////////

class SocketUDP : public ASocket {
  public:
    SocketUDP() = default;
    SocketUDP(const IP &ip, uint16_t port);
    SocketUDP(const SocketUDP &other) = delete;
    SocketUDP &operator=(const SocketUDP &) = delete;
    SocketUDP(SocketUDP &&other);
    SocketUDP &operator=(SocketUDP &&other);
    ~SocketUDP() = default;

    bool send(const UDPMessage &msg, const Address &addr) const;

    /* this seems redundant, but to avoid any heap calls, this is necessary */
    std::pair<UDPMessage, AddressV4> receiveV4(void) const;
    std::pair<UDPMessage, AddressV6> receiveV6(void) const;

  private:
    void receive(struct sockaddr *addr, byte_t *data) const;
};

////////////////////////////////////////

class SocketTCP;

class SocketTCPMaster : public ASocket {
  public:
    SocketTCPMaster() = default;
    SocketTCPMaster(const IP &ip, uint16_t port);

    // don't share anything, all the fields are static
    SocketTCPMaster(const SocketTCPMaster &other) = delete;
    SocketTCPMaster &operator=(const SocketTCPMaster &) = delete;
    SocketTCPMaster(SocketTCPMaster &&other);
    SocketTCPMaster &operator=(SocketTCPMaster &&other);

    ~SocketTCPMaster() = default;

    SocketTCP accept(size_t pos) const;
};

class SocketTCP : public ASocket {
  public:
    enum EventType {
        NONE = 0,
        READ,
        WRITE,
    };

  public:
    SocketTCP(size_t pos_accept,
              const SocketTCPMaster
                  &socketMaster); // accepts it from the socket master
    SocketTCP(const SocketTCP &other) = delete;
    SocketTCP &operator=(const SocketTCP &) = delete;
    SocketTCP(SocketTCP &&other);
    SocketTCP &operator=(SocketTCP &&other);
    ~SocketTCP() = default;

    bool send(const TCPMessage &msg) const;
    TCPMessage receive(void) const;

    std::size_t getPosAccept(void) const { return m_posAccept; }
    const EventType getEventType(void) const { return m_eventType; }

  private:
    std::size_t receiveReliant(byte_t *buffer, std::size_t size) const;
    std::size_t sendReliant(const TCPMessage *msg,
                            std::size_t msgDataSize) const;

    /* since accept can block, set it's here when it will be applied anyway */
    struct sockaddr m_addr;
    socklen_t m_szAddr;

    EventType m_eventType = READ;

    const size_t m_posAccept; /* posititon in array for fast removal */
};

} // namespace Network
