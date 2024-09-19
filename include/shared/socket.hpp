#pragma once

#include <cstdint>

namespace Network {

typedef char byte_t;

class ASocket {

  public:
#ifndef _WIN32
    typedef int SOCKET;
#endif
    virtual std::size_t send(const byte_t *data,
                             const std::size_t size) const = 0;
    virtual std::size_t receive(byte_t *data, const std::size_t size) const = 0;

    static void setBlocking(const SOCKET socket, bool blocking);
    static int socketClose(const SOCKET socket);
};

class SocketUDP : public ASocket {
  public:
    SocketUDP(const std::string &address = "any");
    ~SocketUDP();

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
