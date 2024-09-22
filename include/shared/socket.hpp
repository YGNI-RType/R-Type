#pragma once

#include <cstdint>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h> // Add this line to include the header file that defines sockaddr_in

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
    virtual std::size_t send(const byte_t *data,
                             const std::size_t size) const = 0;
    virtual std::size_t receive(byte_t *data, const std::size_t size) const = 0;

    static void setBlocking(const SOCKET socket, bool blocking);
    static int socketClose(const SOCKET socket);

    static void init(void);

  public:
    virtual ~ASocket() = default;
};

class SocketUDP : public ASocket {
  public:
    SocketUDP(const std::string &address = "any");
    ~SocketUDP() = default;

    std::size_t send(const byte_t *data,
                     const std::size_t size) const override final;
    std::size_t receive(byte_t *data,
                        const std::size_t size) const override final;

    static int initSocket(void);
    static int shutdownSocket(void);

  private:
    static SOCKET mg_sock;

    struct sockaddr_in m_addr;
};

class SocketTCP : public ASocket {
  public:
    SocketTCP();
    ~SocketTCP();

    std::size_t send(const byte_t *data,
                     const std::size_t size) const override final;
    std::size_t receive(byte_t *data,
                        const std::size_t size) const override final;

  private:
    SOCKET m_sock;
};

} // namespace Network
