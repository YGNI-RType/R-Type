/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet (Workspace)
** File description:
** msg_udp_structs
*/

#pragma once

#include <cstdint>

namespace Network {

enum { CL_BROADCAST_PING, SV_BROADCAST_PING, CL_SENDCMD, SV_SNAPSHOT };

struct UDPSV_PingResponse {
    uint16_t tcpv4Port;
    uint16_t tcpv6Port;

    std::uint32_t maxPlayers;
    std::uint32_t currentPlayers;
};

struct UDPG_NetChannelHeader {
    uint64_t sequence;
    uint64_t ack; /* m_udpInSequence */
};

} // namespace Network