/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet (Workspace)
** File description:
** net_address
*/

#pragma once

#include "net_common.hpp"

#include <array>

#ifdef _WIN32
#include <winsock2.h>

typedef uint32_t in_addr_t;

#else
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

namespace Network {
class Address {

protected:
    typedef std::array<byte_t, 4> ipv4_t;
    typedef std::array<byte_t, 16> ipv6_t;

public:
    virtual void toSockAddr(sockaddr *addr) const = 0;
    virtual bool isLanAddr(void) const = 0;

    AddressType getType() const { return m_type; };
    uint16_t getPort() const { return m_port; };

protected:
    Address(AddressType type, uint16_t port) : m_type(type), m_port(port){};
    virtual ~Address() = default;

    AddressType m_type;
    uint16_t m_port;
};

class AddressV4 : public Address {
public:
    AddressV4(AddressType type, uint16_t port, ipv4_t address);
    AddressV4(AddressType type, uint16_t port);
    AddressV4(AddressType type, uint16_t port, in_addr_t ip);
    ~AddressV4() = default;

    const ipv4_t &getAddress() const { return m_address; };

    void toSockAddr(sockaddr *addr) const override final;
    bool isLanAddr(void) const override final;

private:
    ipv4_t m_address;
};

class AddressV6 : public Address {
public:
    AddressV6(AddressType type, uint16_t port, ipv6_t address, uint64_t scopeId);
    AddressV6(AddressType type, uint16_t port);
    AddressV6(AddressType type, uint16_t port, in6_addr ip, uint32_t scopeId);
    ~AddressV6() = default;

    const ipv6_t &getAddress() const { return m_address; };

    void toSockAddr(sockaddr *addr) const override final;
    bool isLanAddr(void) const override final;

private:
    ipv6_t m_address;
    uint64_t m_scopeId;
};
} // namespace Network
