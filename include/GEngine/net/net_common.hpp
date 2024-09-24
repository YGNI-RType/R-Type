/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet (Workspace)
** File description:
** net_common
*/

#pragma once

#include <string>
#include <sys/socket.h>

namespace Network {

#ifndef IF_NAMESIZE
  #define IF_NAMESIZE 16
#endif

#ifdef _WIN32
#ifdef ADDRESS_FAMILY
#define sa_family_t ADDRESS_FAMILY
#else
typedef unsigned short sa_family_t;
#endif
#endif

#define	PORT_ANY    -1
#define	MAX_LISTEN    5
#define MAX_TRY_PORTS 5
#define DEFAULT_PORT 4242

typedef uint8_t byte_t;

typedef enum {
    AT_NONE = 0,
    AT_BOT,
    AT_LOOPBACK,
    AT_BROADCAST,
    AT_MULTICAST,
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
} // namespace Network
