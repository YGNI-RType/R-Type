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
// add multicast for ipv6
// https://en.wikipedia.org/wiki/NACK-Oriented_Reliable_Multicast

#include "shared/net.hpp"
#include "shared/socketError.hpp"
#include "shared/cvar/net.hpp"

#include <cstring>

#ifdef _WIN32
// TODO : remove unused

#include <winsock2.h>
#include <ws2spi.h>
#include <ws2tcpip.h>

typedef int socklen_t;

#define EAGAIN WSAEWOULDBLOCK
#define EADDRNOTAVAIL WSAEADDRNOTAVAIL
#define EAFNOSUPPORT WSAEAFNOSUPPORT
#define ECONNRESET WSAECONNRESET
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
#endif

namespace Network {

/* Global vars */

SocketUDP NET::mg_socketUdp;
SocketTCPMaster NET::mg_socketListenTcp;
SocketUDP NET::mg_socketUdpV6;
SocketTCPMaster NET::mg_socketListenTcpV6;

std::vector<SocketTCP> NET::g_clientSocketsTCP;

std::vector<IP> NET::g_localIPs;

bool NET::enabled = false;

/***************/

void NET::init(void) {
    ASocket::initLibs();

    getLocalAddress();

    uint16_t port = DEFAULT_PORT;

    for (const IP &ip : g_localIPs) { // todo : force an ip, find the best ip
                                      // (privileging pubilc interface)
        if (ip.type == AT_IPV4) {
            mg_socketListenTcp = openSocketTcp(ip, port);
            port++;
            mg_socketUdp = openSocketUdp(ip, port);
            port++;
        }
        if (ip.type == AT_IPV6 && CVar::net_ipv6.getIntValue()) { // check if ipv6 is supported
            mg_socketListenTcpV6 = openSocketTcp(ip, port);
            port++;
            mg_socketUdpV6 = openSocketUdp(ip, port);
            port++;
        }
        break;
    }

    enabled = true;
}

void NET::stop(void) {
    g_clientSocketsTCP.clear();
    g_localIPs.clear(); /* TODO : the pointesr inside the socket class are now
                           UNSAFE, should we copy them instead ? */
    enabled = false;
}

void NET::getLocalAddress(void) {
#if defined(__linux__) || defined(__APPLE__) || defined(__BSD__)
    struct ifaddrs *ifap;

    if (getifaddrs(&ifap))
        return; // throw error

    for (ifaddrs *search = ifap; search; search = search->ifa_next)
        if (ifap->ifa_flags & IFF_UP)
            addLocalAddress(search->ifa_name, search->ifa_addr,
                            search->ifa_netmask);

    freeifaddrs(ifap);

#else
    char hostname[256];
    struct addrinfo hint = {0};
    struct addrinfo *res = NULL;

    numIP = 0;

    if (gethostname(hostname, 256) == SOCKET_ERROR)
        return;

    hint.ai_family = AF_UNSPEC;
    hint.ai_socktype = SOCK_DGRAM;

    if (!getaddrinfo(hostname, NULL, &hint, &res)) {
        struct sockaddr_in mask4 = {0};
        struct sockaddr_in6 mask6 = {0};

        mask4.sin_family = AF_INET;
        std::memset(&mask4.sin_addr.s_addr, 0xFF,
                    sizeof(mask4.sin_addr.s_addr));
        mask6.sin6_family = AF_INET6;
        std::memset(&mask6.sin6_addr, 0xFF, sizeof(mask6.sin6_addr));

        // add all IPs from returned list.
        for (struct addrinfo *search = res; search; search = search->ai_next)
            if (search->ai_family == AF_INET)
                addLocalAddress("", search->ai_addr, (struct sockaddr *)&mask4);
            else if (search->ai_family == AF_INET6)
                addLocalAddress("", search->ai_addr, (struct sockaddr *)&mask6);
    }

    if (res)
        freeaddrinfo(res);
#endif
}

void NET::addLocalAddress(char *ifname, struct sockaddr *sockaddr,
                          struct sockaddr *netmask) {
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

    if (addr.type == AT_LOOPBACK)
        return true;

    else if (addr.type == AT_IPV4) {
        // RFC1918:
        // 10.0.0.0        -   10.255.255.255  (10/8 prefix)
        // 172.16.0.0      -   172.31.255.255  (172.16/12 prefix)
        // 192.168.0.0     -   192.168.255.255 (192.168/16 prefix)
        if (addr.ipv4[0] == 10 || addr.ipv4[0] == 127)
            return true;
        if (addr.ipv4[0] == 172 && addr.ipv4[1] >= 16 && addr.ipv4[1] <= 31)
            return true;
        if (addr.ipv4[0] == 192 && addr.ipv4[1] == 168)
            return true;
    } else if (addr.type == AT_IPV6) {
        if (addr.ipv6[0] == 0xFE && addr.ipv6[1] == 0x80)
            return true;
        if ((addr.ipv6[0] & 0xfe) == 0xfc)
            return true;
    }

    // TODO if time : check if the address is in the same subnet as the local
    return false;
}

SocketTCPMaster NET::openSocketTcp(const IP &ip, uint16_t wantedPort) {
    for (uint16_t i = 0; i < MAX_TRY_PORTS; i++) {
        try {
            return SocketTCPMaster(ip, wantedPort);
        } catch (SocketException &e) {
            continue;
        }
    }
    throw SocketException("Failed to open TCP socket");
}

SocketUDP NET::openSocketUdp(const IP &ip, uint16_t wantedPort) {
    for (uint16_t i = 0; i < MAX_TRY_PORTS; i++) {
        try {
            return SocketUDP(ip, wantedPort);
        } catch (SocketException &e) {
            continue;
        }
    }
    throw SocketException("Failed to open UDP socket");
}

/**************************************************************/

/* returns true if has event, false otherwise */
bool NET::sleep(uint32_t ms) {
    struct timeval timeout = {.tv_sec = ms / 1000,
                              .tv_usec = (ms % 1000) * 1000};
    ASocket::SOCKET highest = ASocket::getHighestSocket();

    fd_set readSet;
    createSets(readSet);

    /* The usage of select : both on windows and unix systems */
    int res = select(highest + 1, &readSet, nullptr, nullptr, &timeout);
    if (res == -1)
        throw SocketException("Failed to sleep");
    else if (res == 0)
        return false;

    // WE GOT SOMETHING GUYS, EVENT
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
    FD_SET(mg_socketListenTcp.getSocket(), &readSet);
}
} // namespace Network
