#pragma once

#include <cstdint>

namespace Network {

typedef char byte_t;

class ASocket {

#ifndef _WIN32
    typedef int SOCKET;
#endif

  public:
    virtual std::size_t send(const byte_t *data, const std::size_t size) const = 0;
    virtual std::size_t receive(byte_t *data, const std::size_t size) const = 0;

    void setBlocking(bool blocking);
    int socketClose(void);

  protected:
    ~ASocket();
    SOCKET m_sock;
};

class SocketUDP : public ASocket {
  public:
    SocketUDP(const std::string &address = "any");

    std::size_t send(const byte_t *data, const std::size_t size) const override final;
    std::size_t receive(byte_t *data, const std::size_t size) const override final;
  private:
    struct sockaddr_in m_addr;
};

class SocketTCP : public ASocket {
  public:
    SocketTCP();

    std::size_t send(const byte_t *data, const std::size_t size) const override final;
    std::size_t receive(byte_t *data, const std::size_t size) const override final;
};

} // namespace Network
