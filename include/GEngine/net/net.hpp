/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet (Workspace)
** File description:
** net
*/

#pragma once

#include "cl_net_client.hpp"
#include "msg.hpp"
#include "net_address.hpp"
#include "net_client.hpp"
#include "net_common.hpp"
#include "net_server.hpp"
#include "socket.hpp"

#include <vector>

namespace Network {

class NET {
private:
    static SocketUDP mg_socketUdp;
    static SocketTCPMaster mg_socketListenTcp;
    static SocketUDP mg_socketUdpV6;
    static SocketTCPMaster mg_socketListenTcpV6;

    static NetServer mg_server;
    static CLNetClient mg_client;

    static std::vector<IP> g_localIPs;

    static bool enabled;

    static bool inittedClient;

    static uint16_t currentUnusedPort;

    /* Init everything */
public:
    static void init(void);
    static void stop(void);

    static void initServer(void);
    static void initClient(void);

private:
    static void getLocalAddress(void);
    static void addLocalAddress(char *ifname, struct sockaddr *sockaddr, struct sockaddr *netmask, bool isLoopback);
    static void sortLocalAddresses(void);
    static bool isLanAddress(const Address &addr);
    /*********************/

    /* Usage of "select" */
public:
    static bool sleep(uint32_t ms);
    static void handleEvents(fd_set &readSet);
    static void handleUdpEvent(SocketUDP &socket, const UDPMessage &msg, const Address &addr);
    static void handleTCPEvent(const TCPMessage &msg);

private:
    static void createSets(fd_set &readSet);
    /*********************/

    /* Ping servers */
public:
    static void pingServers(void);

    static CLNetClient &getClient(void) {
        return mg_client;
    }

private:
};
} // namespace Network
