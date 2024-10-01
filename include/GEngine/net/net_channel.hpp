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

#define NETCHAN_GENCHECKSUM(challenge, sequence) ((challenge) ^ ((sequence) * (challenge)))

namespace Network {

typedef enum {
    CS_FREE,      // can be reused for a new connection
    CS_ZOMBIE,    // client has been disconnected, but don't reuse
                  // connection for a couple seconds
    CS_CONNECTED, // has been assigned to a client_t, but no gamestate yet
    CS_PRIMED,    // gamestate has been sent, but client hasn't sent a usercmd
    CS_ACTIVE     // client is fully in game
} clientState;

typedef enum {
    CON_UNINITIALIZED,
    CON_DISCONNECTED, // not talking to a server
    CON_AUTHORIZING,  // not used any more, was checking cd key
    CON_CONNECTING,   // sending request packets to the server
    CON_CHALLENGING,  // sending challenge packets to the server
    CON_CONNECTED,    // netchan_t established, getting gamestate
    CON_LOADING,      // only during cgame initialization, never during main loop
    CON_PRIMED,       // got gamestate, waiting for first frame
    CON_ACTIVE,       // game views should be displayed
    CON_CINEMATIC     // playing a cinematic or a static pic, not connected to a server
} connectionState;

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
    void setTcpSocket(SocketTCP &&socket) { m_tcpSocket = std::move(socket); }

    bool readDatagram(const UDPMessage &msg, const Address &addr);
    bool readStream(const TCPMessage &msg);

    bool sendDatagram(SocketUDP &socket, UDPMessage &msg, const Address &addr);
    bool sendStream(const TCPMessage &msg);

private:
    const std::unique_ptr<Address> m_toAddress; /* the recast to v6 or v4 is done later */


    /* UDP */

    /* TODO : add delay (?) */
    // PacketPoolUdp m_udpPoolUnsent;

    int m_challenge = -1;


    /* most likely fragments, since the packets may be too big (mostly (always) for client from server) */
    PacketPoolUdp m_udpPoolSend;

    uint32_t m_droppedPackets = 0;

    uint64_t m_udpInSequence = 0;
    uint64_t m_udpOutSequence = 1;
    uint64_t m_udplastsent = 0;
    size_t m_udplastsentsize = 0;


    /*******/

    /* TCP */

    SocketTCP m_tcpSocket;
    /* unsent data (mostly rather small data, downloads are another story)*/
    PacketPoolTcp m_tcpPool;

    /*******/
};
} // namespace Network
