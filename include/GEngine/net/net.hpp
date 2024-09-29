/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet (Workspace)
** File description:
** net
*/

#pragma once

#include "msg.hpp"
#include "net_address.hpp"
#include "net_common.hpp"
#include "socket.hpp"

#include <vector>

namespace Network {

class NET {
private:
    static SocketUDP mg_socketUdp;
    static SocketTCPMaster mg_socketListenTcp;
    static SocketUDP mg_socketUdpV6;
    static SocketTCPMaster mg_socketListenTcpV6;

    static std::vector<SocketTCP> g_clientSocketsTCP;

    static std::vector<IP> g_localIPs;

    static bool enabled;
    static bool isHostingServer;

    /* Init everything */
public:
    static void init(void);
    static void stop(void);

private:
    static void getLocalAddress(void);
    static void addLocalAddress(char *ifname, struct sockaddr *sockaddr, struct sockaddr *netmask);
    static bool isLanAddress(const Address &addr);

    static SocketTCPMaster openSocketTcp(const IP &ip, uint16_t wantedPort);
    static SocketUDP openSocketUdp(const IP &ip, uint16_t wantedPort);
    /*********************/

    /* Usage of "select" */
public:
    static bool sleep(uint32_t ms);
    static void handleEvents(void);

private:
    static void createSets(fd_set &readSet);
    /*********************/

    /* Ping servers */
public:
    static void pingServers(void);
    static void respondPingServers(const UDPMessage &msg, const Address &addr);

private:
};
} // namespace Network
