/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** net_channel
*/

#pragma once

#include "net_msg.hpp"
#include "socket.hpp"

#include <memory>
#include <vector>

// UN udp message / tcp message ne doit pas être envoyé brut,
// il doit être envoyé à travers une pool, qui en fonction du ratelimit enverront tout ça
// Un classe de message spécialisé, donc avec les headers mêem du udp message qui sotn store dans un gros buffer, dont
// les seuls traces sont des structs, précisant leur segmentation.
// la taille du buffer est réplique pour MAXCLIENTS, dans un convar CMD cappé à 100 jouers

#define UDP_POOL_SZ MAX_UDP_MSGLEN * 2
#define TCP_POOL_SZ MAX_TCP_MSGLEN

namespace Network {
class PacketPool {
public:
    PacketPool() = default;
    ~PacketPool() = default;

    void addMessage(const AMessage &msg, byte_t *pool);
    const byte_t *getRawMessage(byte_t *pool) const;

protected:
    std::vector<iomsg> m_iomsgs;
};

class PacketPoolTcp : public PacketPool {
public:
    PacketPoolTcp() = default;
    ~PacketPoolTcp() = default;

private:
    byte_t m_pool[TCP_POOL_SZ];
};

class PacketPoolUdp : public PacketPool {
public:
    PacketPoolUdp() = default;
    ~PacketPoolUdp() = default;

private:
    byte_t m_pool[UDP_POOL_SZ];
};

/*
    server : used for each NetClient
    client : used only to send cmd in server
*/
class NetChannel {
public:
    NetChannel() = default;
    NetChannel(std::unique_ptr<Address> addr, SocketTCP &socket);
    ~NetChannel() = default;

    const SocketTCP &getTcpSocket(void) const { return m_tcpSocket; }

private:
    const std::unique_ptr<Address> m_toAddress; /* the recast to v6 or v4 is done later */

    /* UDP */

    PacketPoolUdp m_udpPoolUnsent;
    /* most likely fragments, since the packets may be too big (mostly for client from server) */
    PacketPoolUdp m_udpPoolReceived;

    uint32_t m_droppedPackets = 0;

    /*******/

    /* TCP */

    SocketTCP m_tcpSocket;
    /* unsent data (mostly rather small data, downloads are another story)*/
    PacketPoolTcp m_tcpPool;

    /*******/
};
} // namespace Network
