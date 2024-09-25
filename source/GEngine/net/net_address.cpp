/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet (Workspace)
** File description:
** net_address
*/

#include "GEngine/net/net_address.hpp"

namespace Network {

AddressV4::AddressV4(AddressType type, uint16_t port, ipv4_t address)
    : Address(type, port), m_address(address) {};
AddressV4::AddressV4(AddressType type, uint16_t port) : Address(type, port) {};

AddressV4::AddressV4(AddressType type, uint16_t port, in_addr_t ip)
    : Address(type, port) {
    m_address = *(ipv4_t *)&ip;
}

void AddressV4::toSockAddr(sockaddr *addr) const {
    struct sockaddr_in *s = reinterpret_cast<struct sockaddr_in *>(addr);

    s->sin_family = AF_INET;
    s->sin_port = m_port;

    switch (m_type) {
    case AT_IPV4:
        s->sin_addr.s_addr = *(int *)&m_address;
        break;
    case AT_BROADCAST:
        s->sin_addr.s_addr = INADDR_BROADCAST;
        break;
    case AT_LOOPBACK:
        s->sin_addr.s_addr = INADDR_LOOPBACK;
        break;
    case AT_UNSPEC:
        s->sin_addr.s_addr = INADDR_ANY;
        break;
    }
}

bool AddressV4::isLanAddr(void) const {
    // RFC1918:
    // 10.0.0.0        -   10.255.255.255  (10/8 prefix)
    // 172.16.0.0      -   172.31.255.255  (172.16/12 prefix)
    // 192.168.0.0     -   192.168.255.255 (192.168/16 prefix)
    if (m_address[0] == 10 || m_address[0] == 127)
        return true;
    if (m_address[0] == 172 && m_address[1] >= 16 && m_address[1] <= 31)
        return true;
    if (m_address[0] == 192 && m_address[1] == 168)
        return true;
    return false;
}

/**********************************************/

AddressV6::AddressV6(AddressType type, uint16_t port, ipv6_t address,
                     uint64_t scopeId)
    : Address(type, port), m_address(address), m_scopeId(scopeId) {};
AddressV6::AddressV6(AddressType type, uint16_t port) : Address(type, port) {};

AddressV6::AddressV6(AddressType type, uint16_t port, in6_addr ip, uint32_t scopeId)
    : Address(type, port), m_scopeId(scopeId) {
    m_address = *(ipv6_t *)&ip;
    m_scopeId = scopeId;
}

void AddressV6::toSockAddr(sockaddr *addr) const {
    struct sockaddr_in6 *s = reinterpret_cast<struct sockaddr_in6 *>(addr);

    s->sin6_family = AF_INET6;
    s->sin6_port = m_port;

    switch (m_type) {
    case AT_IPV6:
        s->sin6_addr = *((struct in6_addr *)&m_address);
        s->sin6_scope_id = m_scopeId;
        break;
    case AT_MULTICAST:
    case AT_UNSPEC:
        s->sin6_addr = in6addr_any;
        break;
    case AT_LOOPBACK:
        s->sin6_addr = in6addr_loopback;
        break;
    }
}

bool AddressV6::isLanAddr(void) const {
    // RFC4193:
    // fc00::/7
    // fd00::/8
    if (m_address[0] == 0xFE && m_address[1] == 0x80)
        return true;
    if ((m_address[0] & 0xfe) == 0xfc)
        return true;
    return false;
}

} // namespace Network