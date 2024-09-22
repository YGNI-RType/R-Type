/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet (Workspace)
** File description:
** net
*/

#pragma once

#include "socket.hpp"

#include <vector>
#include <array>

namespace Network {

#ifndef IF_NAMESIZE
  #define IF_NAMESIZE 16
#endif

typedef enum {
    AT_NONE = 0,
    AT_BOT,
    AT_LOOPBACK,
    AT_BROADCAST,
    AT_IPV4,
    AT_IPV6,
    AT_UNSPEC
} AddressType;

struct IP {
    std::string ifname = std::string(IF_NAMESIZE, '\0');

    AddressType type;
    sa_family_t family;
    struct sockaddr_storage addr;
    struct sockaddr_storage netmask;
};

struct Address {
    AddressType type;
    std::array<byte_t, 4> ipv4;
    std::array<byte_t, 16> ipv6;
    uint16_t port;
    uint64_t interface_id; // scope_id for ipv6 / zone_id
};

class NET {
  public:
    static SocketUDP g_socketUdp;
    static SocketTCP g_socketListenTdp;

    static std::vector<IP> g_localIPs;

    /* Init everything */
  public:
    static void init(void);

  private:
    static void getLocalAddress(void);
    static void addLocalAddress(char *ifname, struct sockaddr *sockaddr,
                                struct sockaddr *netmask);
    static bool isLanAddress(const Address &addr);
    /*********************/
};
} // namespace Network
