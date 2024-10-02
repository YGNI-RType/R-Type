/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** net_channel
*/

#include "GEngine/net/net_channel.hpp"
#include "GEngine/cvar/net.hpp"
#include "GEngine/time/time.hpp"

namespace Network {

NetChannel::NetChannel(bool isServer, std::unique_ptr<Address> clientAddress, SocketTCP &socket)
    : m_enabled(true), m_toAddress(std::move(clientAddress)), m_tcpSocket(std::move(socket)) {
    if (!isServer)
        return;

    /* each has challenge, avoid packet hi-jacking (todo (?) challenge are used for ddos, but is that the system
     * preventing it instead of us ?) */
    m_challenge = (((unsigned int)rand() << 16) ^ (unsigned int)rand()) ^ Time::Clock::milliseconds();
}

bool NetChannel::sendDatagram(SocketUDP &socket, UDPMessage &msg, const Address &addr) {
    size_t msgLen = msg.getSize();

    /* check the client rating before or after ? */

    // rah shit, fragment too big. put the rest in the pool in case of network rating issue
    if (msgLen > FRAGMENT_SIZE) {
        // TODO : fragment the message, put in a sending pool
        return false;
    }

    msg.writeHeader({.sequence = NETCHAN_GENCHECKSUM(m_challenge, m_udpOutSequence)});

    size_t sent = socket.send(msg, addr);
    if (sent < 0) // guess it's send, but not quite, TODO : check any weird case (place breakpoint)
        return true;

    m_udpOutSequence++;

    /* for client/server rates calculation */
    m_udplastsent = Time::Clock::milliseconds();
    m_udplastsentsize = sent;
    return true;
}

bool NetChannel::readDatagram(const UDPMessage &msg, const Address &addr) {
    UDPG_NetChannelHeader header;
    msg.readHeader(header);

    if (!NETCHAN_GENCHECKSUM(m_challenge, header.sequence))
        return false; /* what is going on sir ???? */

    if (header.sequence <= m_udpInSequence) {
        /*out of order packet, delete it */
        return false;
    }
    m_droppedPackets = header.sequence - m_udpInSequence + 1;

    /****** At this point, the packet is valid *******/

    m_udplastrecv = Time::Clock::milliseconds();

    if (msg.isFragmented()) {
        /* todo : add to pool of receiving packet */

        /* if it's the end of the fragmented data, return true */
        return false;
    }

    m_udpInSequence = header.sequence;
    return true;
}

bool NetChannel::isTimeout() const {
    return Time::Clock::milliseconds() - m_udplastrecv > CVar::net_kick_timeout.getIntValue();
}

} // namespace Network