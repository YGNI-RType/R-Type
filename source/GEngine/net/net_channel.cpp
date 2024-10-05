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
    : m_enabled(true)
    , m_toAddress(std::move(clientAddress))
    , m_tcpSocket(std::move(socket)) {
    if (!isServer)
        return;

    /* each has challenge, avoid packet hi-jacking (todo (?) challenge are used for ddos, but is that the system
     * preventing it instead of us ?) */
    m_challenge = (((unsigned int)rand() << 16) ^ (unsigned int)rand()) ^ Time::Clock::milliseconds();
}

bool NetChannel::sendDatagram(SocketUDP &socket, UDPMessage &msg, const Address &addr) {
    size_t msgLen = msg.getSize();

    /* check the client rating before or after ? */

    uint64_t &udpOutSequence = msg.shouldAck() ? m_udpACKOutSequence : m_udpOutSequence;
    uint64_t &udpInSequence = msg.shouldAck() ? m_udpACKInSequence : m_udpInSequence;

    // rah shit, fragment too big. put the rest in the pool in case of network rating issue
    if (msgLen > MAX_UDP_PACKET_LENGTH) {
        m_udpPoolSend.addMessage(m_udpMyFragSequence, msg);

        auto fragments = m_udpPoolSend.getMissingFragments(m_udpMyFragSequence, 0);
        auto [msgType, maxSize, lastChunkSz, _] = m_udpPoolSend.getMsgSequenceInfo(m_udpMyFragSequence);
        uint8_t i = 0;

        /* todo : add rate limit and all, only do it once though */
        for (const auto &chunk : fragments) {
            UDPMessage newMsg(false, msgType);

            newMsg.setFragmented(true);
            UDPG_FragmentHeaderTo fragHeader;
            fragHeader.idSequence = m_udpMyFragSequence;
            fragHeader.fragId = i;
            fragHeader.fragIdMax = maxSize;

            m_udpPoolSend.constructMessage(newMsg, chunk, fragments.back() == chunk ? lastChunkSz : chunk->size(),
                                           fragHeader);
            sendDatagram(socket, newMsg, addr);
            i++;
        }

        return false;
    }

    UDPG_NetChannelHeader header = {.sequence = NETCHAN_GENCHECKSUM(m_challenge, udpOutSequence),
                                    .ackFragmentSequence = m_udpMyFragSequence};
    if (msg.shouldAck())
        header.ack = udpInSequence;
    msg.writeHeader(header);

    size_t sent = socket.send(msg, addr);
    if (sent < 0) // guess it's send, but not quite, TODO : check any weird case (place breakpoint)
        return true;

    udpOutSequence++;

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

    uint64_t &udpOutSequence = msg.shouldAck() ? m_udpACKOutSequence : m_udpOutSequence;
    uint64_t &udpInSequence = msg.shouldAck() ? m_udpACKInSequence : m_udpInSequence;

    if (header.sequence <= udpOutSequence) {
        /*out of order packet, delete it */
        return false;
    }

    if (msg.shouldAck()) { /* only care about reliable packets */
        m_udpACKClientLastACK = header.ack;
        m_droppedPackets = header.sequence - udpInSequence + 1;
    }

    /****** At this point, the packet is valid *******/

    m_udplastrecv = Time::Clock::milliseconds();
    m_udpFromFragSequence = header.ackFragmentSequence;

    if (msg.isFragmented()) {
        /* todo : add to pool of receiving packet */

        /* if it's the end of the fragmented data, return true */
        return false;
    }

    udpInSequence = header.sequence;
    return true;
}

bool NetChannel::sendStream(const TCPMessage &msg) {
    size_t sent = m_tcpSocket.send(msg);
    if (sent < 0)
        return false;

    return true;
}

bool NetChannel::readStream(TCPMessage &msg) {
    /* todo : add header check etc */

    m_tcpSocket.receive(msg);
    return true;
}

bool NetChannel::isTimeout() const {
    return Time::Clock::milliseconds() - m_udplastrecv > CVar::net_kick_timeout.getIntValue();
}

} // namespace Network