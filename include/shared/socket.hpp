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

    SocketTCP accept(void) const;
    SOCKET getSocket(void) const { return mg_sock; }

  private:
    static SOCKET mg_sock;

    const IP *m_ip;
};

class SocketTCP : public ASocket, public ISocketIO {
  public:
    SocketTCP() = default;
    SocketTCP(const SocketTCPMaster
                  &socketMaster); // accepts it from the socket master
    ~SocketTCP();

    std::size_t send(const byte_t *data,
                     const std::size_t size) const override final;
    std::size_t receive(byte_t *data,
                        const std::size_t size) const override final;

  private:
    SOCKET m_sock = -1;
};

} // namespace Network
