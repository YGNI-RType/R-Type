/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet (Workspace)
** File description:
** net
*/

#pragma once

#include "net_common.hpp"
#include "socket.hpp"

#include <vector>
#include <array>
#include <memory>

namespace Network {

struct Address {
    AddressType type;
    std::array<byte_t, 4> ipv4;
    std::array<byte_t, 16> ipv6;
    uint16_t port;
    uint64_t interface_id; // scope_id for ipv6 / zone_id
};

class NET {
  public:
    static std::unique_ptr<SocketUDP> g_socketUdp;
    static std::unique_ptr<SocketTCP> g_socketListenTdp;

    static std::vector<IP> g_localIPs;

    /* Init everything */
  public:
    static void init(void);

  private:
    static void getLocalAddress(void);
    static void addLocalAddress(char *ifname, struct sockaddr *sockaddr,
                                struct sockaddr *netmask);
    static bool isLanAddress(const Address &addr);

    static SocketTCPMaster openSocketTcp(const IP &ip, uint16_t wantedPort);
    static SocketUDP openSocketUdp(const IP &ip, uint16_t wantedPort);
    /*********************/
};
} // namespace Network
