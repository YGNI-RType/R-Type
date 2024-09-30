/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet (Workspace)
** File description:
** net
*/

// udp : why  the chunks are 1400 bytes long?
// https://stackoverflow.com/questions/1098897/what-is-the-largest-safe-udp-packet-size-on-the-internet

// for tcp : reliable messages (chat, image upload/download, game changes (map
// update, changing connection state of client)) for udp : unreliable messages
// (snapshot deltas/(full because dummy is set to all 0, meaning delta = full),
// voip)

// open both UDP socket for ipv4 and ipv6 for the same class
// add multicast for ipv6 --> NOT PRIORITARY
// https://en.wikipedia.org/wiki/NACK-Oriented_Reliable_Multicast

#include "GEngine/net/net.hpp"
#include "GEngine/cvar/net.hpp"
#include "GEngine/net/msg.hpp"
#include "GEngine/net/socketError.hpp"

#include <cstring>

// temp
#include <iostream>

#ifdef _WIN32
// TODO : remove unused

#include <winsock2.h>
#include <ws2spi.h>
#include <ws2tcpip.h>

typedef int socklen_t;
typedef u_long ioctlarg_t;

#define socketError WSAGetLastError()

#else
#include <arpa/inet.h>
#include <errno.h>
#include <net/if.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#if !defined(__sun) && !defined(__sgi)
#include <ifaddrs.h>
#endif

#define socketError errno

#endif

namespace Network {

/* Global vars */

SocketUDP NET::mg_socketUdp;
SocketTCPMaster NET::mg_socketListenTcp;
SocketUDP NET::mg_socketUdpV6;
SocketTCPMaster NET::mg_socketListenTcpV6;

std::vector<SocketTCP> NET::g_clientSocketsTCP;

std::vector<IP> NET::g_localIPs;

uint16_t NET::currentUnusedPort = DEFAULT_PORT;

bool NET::enabled = false;
bool NET::inittedServer = false;
bool NET::inittedClient = false;

/***************/

void NET::init(void) {
    ASocket::initLibs();

    getLocalAddress();

    for (const IP &ip : g_localIPs) { // todo : force an ip, find the best ip
                                      // (privileging pubilc interface)
        if (ip.type == AT_IPV4) {
            mg_socketUdp = openSocketUdp(ip, currentUnusedPort);
            currentUnusedPort++;
        }
        if (ip.type == AT_IPV6 && CVar::net_ipv6.getIntValue()) { // check if ipv6 is supported
            mg_socketUdpV6 = openSocketUdp(ip, currentUnusedPort);
            currentUnusedPort++;
        }
        break;
    }

    enabled = true;
}

void NET::initServer(void) {
    if (!NET::enabled)
        return;

    // TODO : cloes everything if already initted
    if (inittedServer)
        return;

    for (const IP &ip : g_localIPs) { // todo : force an ip, find the best ip
                                      // (privileging pubilc interface)
        if (ip.type == AT_IPV4) {
            mg_socketListenTcp = openSocketTcp(ip, currentUnusedPort);
            currentUnusedPort++;
        }
        if (ip.type == AT_IPV6 && CVar::net_ipv6.getIntValue()) { // check if ipv6 is supported
            mg_socketListenTcpV6 = openSocketTcp(ip, currentUnusedPort);
            currentUnusedPort++;
        }
        break;
    }

    inittedServer = true;
}

void NET::stop(void) {
    g_clientSocketsTCP.clear();
    g_localIPs.clear();
    enabled = false;
}

void NET::getLocalAddress(void) {
#if defined(__linux__) || defined(__APPLE__) || defined(__BSD__)
    struct ifaddrs *ifap;

    if (getifaddrs(&ifap))
        return; // throw error

    for (ifaddrs *search = ifap; search; search = search->ifa_next)
        if (ifap->ifa_flags & IFF_UP)
            addLocalAddress(search->ifa_name, search->ifa_addr, search->ifa_netmask);

    freeifaddrs(ifap);

#else
    char hostname[256];
    struct addrinfo hint = {0};
    struct addrinfo *res = NULL;

    size_t numIP = 0;

    if (gethostname(hostname, 256) == SOCKET_ERROR)
        return;

    hint.ai_family = AF_UNSPEC;
    hint.ai_socktype = SOCK_DGRAM;

    if (!getaddrinfo(hostname, NULL, &hint, &res)) {
        struct sockaddr_in mask4 = {0};
        struct sockaddr_in6 mask6 = {0};

        mask4.sin_family = AF_INET;
        std::memset(&mask4.sin_addr.s_addr, 0xFF, sizeof(mask4.sin_addr.s_addr));
        mask6.sin6_family = AF_INET6;
        std::memset(&mask6.sin6_addr, 0xFF, sizeof(mask6.sin6_addr));

        // add all IPs from returned list.
        for (struct addrinfo *search = res; search; search = search->ai_next)
            if (search->ai_family == AF_INET)
                addLocalAddress((char *)"", search->ai_addr, (struct sockaddr *)&mask4);
            else if (CVar::net_ipv6.getIntValue() && search->ai_family == AF_INET6)
                addLocalAddress((char *)"", search->ai_addr, (struct sockaddr *)&mask6);
    }

    if (res)
        freeaddrinfo(res);
#endif
}

void NET::addLocalAddress(char *ifname, struct sockaddr *sockaddr, struct sockaddr *netmask) {
    // only add addresses that have all required info.
    if (!sockaddr || !netmask || !ifname)
        return;

    sa_family_t family = sockaddr->sa_family;
    size_t addrlen;

    switch (family) {
    case AF_INET:
        addrlen = sizeof(struct sockaddr_in);
        break;
    case AF_INET6:
        addrlen = sizeof(struct sockaddr_in6);
        break;
    default:
        return;
    }

    IP ip = {.family = sockaddr->sa_family};

    if (family == AF_INET) {
        addrlen = sizeof(struct sockaddr_in);
        ip.type = AT_IPV4;
    } else if (family == AF_INET6) {
        addrlen = sizeof(struct sockaddr_in6);
        ip.type = AT_IPV6;
    } else
        return;

    ip.ifname = ifname;

    std::memcpy(&ip.addr, sockaddr, addrlen);
    std::memcpy(&ip.netmask, netmask, addrlen);

    return g_localIPs.push_back(ip);
}

bool NET::isLanAddress(const Address &addr) {

    if (addr.getType() == AT_LOOPBACK)
        return true;

    return addr.isLanAddr();
}

SocketTCPMaster NET::openSocketTcp(const IP &ip, uint16_t wantedPort) {
    for (uint16_t i = 0; i < MAX_TRY_PORTS; i++) {
        try {
            return std::move(SocketTCPMaster(ip, wantedPort + i));
        } catch (SocketException &e) {
            if (!e.shouldRetry() || i == MAX_TRY_PORTS - 1)
                throw e;
            continue;
        }
    }
    throw SocketException("Failed to open TCP socket");
}

SocketUDP NET::openSocketUdp(const IP &ip, uint16_t wantedPort) {
    for (uint16_t i = 0; i < MAX_TRY_PORTS; i++) {
        try {
            return std::move(SocketUDP(ip, wantedPort + i));
        } catch (SocketException &e) {
            if (!e.shouldRetry() || i == MAX_TRY_PORTS - 1)
                throw e;
            continue;
        }
    }
    throw SocketException("Failed to open UDP socket");
}

/**************************************************************/

/* returns true if has event, false otherwise */
bool NET::sleep(uint32_t ms) {
    struct timeval timeout = {.tv_sec = static_cast<long>(ms / 1000u), .tv_usec = static_cast<int>((ms % 1000) * 1000)};
    SOCKET highest = ASocket::getHighestSocket();

    fd_set readSet;
    createSets(readSet);

    /* The usage of select : both on windows and unix systems */
    int res = select(highest + 1, &readSet, nullptr, nullptr, &timeout);
    if (res == -1)
        throw SocketException(strerror(socketError));
    else if (res == 0)
        return false;

    handleEvents(readSet);
    return true;
}

void NET::createSets(fd_set &readSet) {
    FD_ZERO(&readSet);

    for (SocketTCP &socket : g_clientSocketsTCP) {
        auto eventType = socket.getEventType();
        auto socketId = socket.getSocket();

        if (eventType == SocketTCP::EventType::READ)
            FD_SET(socketId, &readSet);
    }

    if (CVar::net_ipv6.getIntValue()) {
        FD_SET(mg_socketUdpV6.getSocket(), &readSet);
        FD_SET(mg_socketListenTcpV6.getSocket(), &readSet);
    }
    FD_SET(mg_socketUdp.getSocket(), &readSet);
    if (inittedServer)
        FD_SET(mg_socketListenTcp.getSocket(), &readSet);
}

void NET::handleUdpEvent(const UDPMessage &msg, const Address &addr)
{
    switch (msg.getType()) {
        case CL_BROADCAST_PING:
            if (NET::inittedServer) {
                std::cout << "SV: Received ping" << std::endl;
                NET::respondPingServers(msg, addr);
            }
            break;
        case SV_BROADCAST_PING:
            std::cout << "CL: received ping response" << std::endl;
            return;
            // handlePingResponse(msg, addr);
        default: // handleUdpMessage(msg, addr);
            break;
    }
}

void NET::handleEvents(fd_set &readSet) {
    if (FD_ISSET(mg_socketUdp.getSocket(), &readSet)) {
        UDPMessage msg(0, 0);
        auto addr = mg_socketUdp.receiveV4(msg);
        return NET::handleUdpEvent(msg, addr);
    }
    if (NET::inittedServer) {
        if (FD_ISSET(mg_socketListenTcp.getSocket(), &readSet))
            return;
            // mg_socketListenTcp.handleEvent();
    }
    if (CVar::net_ipv6.getIntValue()) {
        if (FD_ISSET(mg_socketUdpV6.getSocket(), &readSet))
            // mg_socketUdpV6.handleEvent();
            return;
        if (NET::inittedServer) {
            if (FD_ISSET(mg_socketListenTcpV6.getSocket(), &readSet))
                // mg_socketListenTcpV6.handleEvent();
                return;
        }
    }
    for (SocketTCP &socket : g_clientSocketsTCP) {
        auto eventType = socket.getEventType();
        auto socketId = socket.getSocket();

        if (eventType == SocketTCP::EventType::READ && FD_ISSET(socketId, &readSet))
            return;
        //     socket.handleEvent();
    }
}

/**************************************************************/

/* should it be bool ? should it returns a message instead of sending it directly ? */
void NET::pingServers(void) {
    for (uint16_t port = DEFAULT_PORT; port < DEFAULT_PORT + MAX_TRY_PORTS; port++) {
        auto message = UDPMessage(0, CL_BROADCAST_PING);
        std::cout << "CL: sending ping broadcast to port " << port << std::endl;
        mg_socketUdp.send(message, AddressV4(AT_BROADCAST, port));
        if (CVar::net_ipv6.getIntValue())
            mg_socketUdpV6.send(message, AddressV6(AT_MULTICAST, port));
    }
}

void NET::respondPingServers(const UDPMessage &msg, const Address &addr) {
    auto pingReponseMsg = UDPMessage(0, SV_BROADCAST_PING);
    std::cout << "SV: respond to ping, sending to port " << addr.getPort() << std::endl;
    if (addr.getType() == AT_IPV4)
        mg_socketUdp.send(pingReponseMsg, addr);
    else if (CVar::net_ipv6.getIntValue())
        mg_socketUdpV6.send(pingReponseMsg, addr);
}

} // namespace Network
