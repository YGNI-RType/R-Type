/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet (Workspace)
** File description:
** socket
*/

#pragma once

#include "net_common.hpp"

#include <cstdint>
#include <netinet/in.h> // Add this line to include the header file that defines sockaddr_in
#include <string>
#include <sys/socket.h>
#include <sys/types.h>

namespace Network {

typedef uint8_t byte_t;

#define MAX_PACKETLEN 1400 // max size of a network packet

#define FRAGMENT_SIZE (MAX_PACKETLEN - 100)
#define PACKET_HEADER 10 // two ints and a short

#define UDP_SO_RCVBUF_SIZE 131072

////////////////////////////////////////

class ASocket {

  public:
#ifndef _WIN32
    typedef int SOCKET;
#else
    static WSADATA winsockdata;
#endif

    static void setBlocking(const SOCKET socket, bool blocking);
    static int socketClose(const SOCKET socket);
    static SOCKET getHighestSocket(void) { return m_highFd; }

    virtual SOCKET getSocket(void) const = 0;

  public:
    static void initLibs(void);

  protected:
    static void addSocketPool(SOCKET socket);
    virtual ~ASocket() = default;

  private:
    static fd_set m_fdSet;
    static SOCKET m_highFd;
};

class ISocketIO {
  public:
    virtual std::size_t send(const byte_t *data,
                             const std::size_t size) const = 0;
    virtual std::size_t receive(byte_t *data, const std::size_t size) const = 0;
};

////////////////////////////////////////

class SocketUDP : public ASocket, public ISocketIO {
  public:
    SocketUDP() = default;
    SocketUDP(const IP &ip, uint16_t port);
    ~SocketUDP();

    std::size_t send(const byte_t *data,
                     const std::size_t size) const override final;
    std::size_t receive(byte_t *data,
                        const std::size_t size) const override final;

    SOCKET getSocket(void) const override final { return mg_sock; }

  private:
    static SOCKET mg_sock;

    const IP *m_ip;
};

////////////////////////////////////////

class SocketTCP;

class SocketTCPMaster : public ASocket {
  public:
    SocketTCPMaster() = default;
    SocketTCPMaster(const IP &ip, uint16_t port);
    ~SocketTCPMaster();

    SocketTCP accept(size_t pos) const;
    SOCKET getSocket(void) const override final { return mg_sock; }
    const IP &getIP(void) const { return *m_ip; }

  private:
    static SOCKET mg_sock;

    const IP *m_ip;
};

class SocketTCP : public ASocket, public ISocketIO {
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
    ~SocketTCP();

    std::size_t send(const byte_t *data,
                     const std::size_t size) const override final;
    std::size_t receive(byte_t *data,
                        const std::size_t size) const override final;

    std::size_t getPosAccept(void) const { return m_posAccept; }
    SOCKET getSocket(void) const override final { return m_sock; }
    const EventType getEventType(void) const { return m_eventType; }

  private:
    SOCKET m_sock = -1;

    const IP *m_ip;
    EventType m_eventType = READ;

    const size_t m_posAccept; /* posititon in array for fast removal */
};

} // namespace Network
