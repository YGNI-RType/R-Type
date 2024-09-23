/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet (Workspace)
** File description:
** net
*/

#pragma once

#include "net_common.hpp"
#include "socket.hpp"

#include <array>
#include <memory>
#include <vector>

namespace Network {

struct Address {
    AddressType type;
    std::array<byte_t, 4> ipv4;
    std::array<byte_t, 16> ipv6;
    uint16_t port;
    uint64_t interface_id; // scope_id for ipv6 / zone_id
};

class NET {
  private:
    static SocketUDP mg_socketUdp;
    static SocketTCPMaster mg_socketListenTcp;
    static SocketUDP mg_socketUdpV6;
    static SocketTCPMaster mg_socketListenTcpV6;
    
    static std::vector<SocketTCP> g_clientSocketsTCP;


    static std::vector<IP> g_localIPs;

    static bool enabled;

    /* Init everything */
  public:
    static void init(void);
    static void stop(void);

  private:
    static void getLocalAddress(void);
    static void addLocalAddress(char *ifname, struct sockaddr *sockaddr,
                                struct sockaddr *netmask);
    static bool isLanAddress(const Address &addr);

    static SocketTCPMaster openSocketTcp(const IP &ip, uint16_t wantedPort);
    static SocketUDP openSocketUdp(const IP &ip, uint16_t wantedPort);
    /*********************/

  public:
    static bool sleep(uint32_t ms);

  private:
    static void createSets(fd_set &readSet);
};
} // namespace Network
