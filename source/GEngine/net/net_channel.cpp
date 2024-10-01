/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** net_channel
*/

#include "GEngine/net/net_channel.hpp"
#include "GEngine/time/time.hpp"

namespace Network {

NetChannel::NetChannel(std::unique_ptr<Address> clientAddress, SocketTCP &socket)
    : m_toAddress(std::move(clientAddress)), m_tcpSocket(std::move(socket)) {}

bool NetChannel::sendDatagram(SocketUDP &socket, UDPMessage &msg, const Address &addr) {
    size_t msgLen = msg.getSize();

    /* check the client rating before or after ? */

    // rah shit, fragment too big. put the rest in the pool in case of network rating issue
    if (msgLen > FRAGMENT_SIZE) {
        // TODO : fragment the message, put in a sending pool
        return false;
    }

    msg.writeHeader({.challenge = m_challenge, .sequence = m_udpOutSequence});

    size_t sent = socket.send(msg, addr);
    if (sent < 0) // guess it's send, but not quite, TODO : check any weird case (place breakpoint)
        return true;

    m_udpOutSequence++;

    /* for client/server rates calculation */
    m_udplastsent = Time::Clock::milliseconds();
    m_udplastsentsize = sent;
    return true;
}

bool readDatagram(const UDPMessage &msg, const Address &addr)
{
    
}

} // namespace Network