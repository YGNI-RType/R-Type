/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet (Workspace)
** File description:
** msg_udp_structs
*/

#pragma once

#include <cstdint>

namespace Network {

enum {
    CL_BROADCAST_PING,
    SV_BROADCAST_PING
};

struct UDPSV_PingResponse {
    uint16_t tcpv4Port;
    uint16_t tcpv6Port;

    std::size_t maxPlayers;
    std::size_t currentPlayers;
};

} // namespace Network