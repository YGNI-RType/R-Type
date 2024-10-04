/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** cl_net_client
*/

#include "GEngine/net/cl_net_client.hpp"

#include <iostream> // todo : remove

namespace Network {

bool CLNetClient::connectToServer(size_t index) {

    /* Connects to something */
    // try {
    if (index >= m_pingedServers.size())
        return false;

    std::cout << "CL: connecting to server" << std::endl;

    auto &[response, addr] = m_pingedServers.at(index);
    auto sock = addr->getType() == AT_IPV4 ? SocketTCP(static_cast<AddressV4 &>(*addr), response.tcpv4Port)
                                           : SocketTCP(static_cast<AddressV6 &>(*addr), response.tcpv6Port);

    sock.setBlocking(false);
    m_netChannel = std::move(NetChannel(false, std::move(addr), sock));
    //     /* todo : some error handling just in case ? */

    m_state = CS_CONNECTED;
    m_connectionState = CON_CONNECTING;
    return true;
}

bool CLNetClient::connectToServer(const std::string &ip, uint16_t port, bool block) {
    std::unique_ptr<Address> addr;

    try {
        addr = std::make_unique<AddressV4>(AT_IPV4, ip, port);
    } catch (const std::exception &e) {
        try {
            addr = std::make_unique<AddressV6>(AT_IPV6, ip, port);
        } catch (const std::exception &e) {
            return false;
        }
    }

    /* fix : when the addres is bad, it holds since it blocks, make it unblock by default ? */
    auto sock = addr->getType() == AT_IPV4 ? SocketTCP(static_cast<AddressV4 &>(*addr), port, block)
                                           : SocketTCP(static_cast<AddressV6 &>(*addr), port, block);

    std::cout << "connecting is not ready ?: " << sock.isNotReady() << std::endl;
    m_netChannel = std::move(NetChannel(false, std::move(addr), sock));
    //     /* todo : some error handling just in case ? */

    m_state = CS_CONNECTED;
    m_connectionState = CON_CONNECTING;
    return true;
}

void CLNetClient::disconnectFromServer(void) {
    m_netChannel.setTcpSocket(SocketTCP());

    m_state = CS_FREE;
    m_connectionState = CON_DISCONNECTED;
}

void CLNetClient::createSets(fd_set &readSet) {
    if (!m_enabled || !m_netChannel.isEnabled())
        return;

    m_netChannel.getTcpSocket().setFdSet(readSet);
}

void CLNetClient::init(void) {
    m_enabled = true;
}

void CLNetClient::stop(void) {
    m_enabled = false;
}

bool CLNetClient::handleUDPEvents(SocketUDP &socket, const UDPMessage &msg, const Address &addr) {
    if (!m_enabled)
        return false;

    switch (msg.getType()) {
    case SV_BROADCAST_PING:
        getPingResponse(msg, addr);
        std::cout << "CL: got ping response !!" << std::endl;
        return true;
    default:
        return handleServerUDP(socket, msg, addr);
    }
}

bool CLNetClient::handleServerUDP(SocketUDP &socket, const UDPMessage &msg, const Address &addr) {
    if (!m_netChannel.isEnabled() ||
        addr != m_netChannel.getAddress()) // why sending udp packets to the client ? who are you ?
        return false;

    std::cout << "CL: received command !!" << std::endl;
    return true;
}

bool CLNetClient::handleTCPEvents(fd_set &readSet) {
    if (!m_enabled || !m_netChannel.isEnabled())
        return false;

    if (m_netChannel.getTcpSocket().isFdSet(readSet)) {
        TCPMessage msg(0, 0);
        if (!m_netChannel.readStream(msg))
            return false;

        return handleServerTCP(msg);
    }
    return false;
}

bool CLNetClient::handleServerTCP(const TCPMessage &msg) {
    switch (msg.getType()) {
    case SV_INIT_CONNECTON:
        TCPSV_ClientInit data;
        msg.readData<TCPSV_ClientInit>(data);

        m_netChannel.setChallenge(data.challenge);
        std::cout << "CL: Client challange: " << data.challenge << std::endl;
        m_connectionState = CON_AUTHORIZING;
        return true;
    default:
        return false;
    }
    return true;
}

void CLNetClient::getPingResponse(const UDPMessage &msg, const Address &addr) {
    UDPSV_PingResponse data;
    msg.readData<UDPSV_PingResponse>(data);

    std::unique_ptr<Address> addrPtr;
    if (addr.getType() == AT_IPV4)
        addrPtr = std::make_unique<AddressV4>(static_cast<const AddressV4 &>(addr));
    else if (addr.getType() == AT_IPV6)
        addrPtr = std::make_unique<AddressV6>(static_cast<const AddressV6 &>(addr));

    m_pingedServers.push_back({data, std::move(addrPtr)});
}

void CLNetClient::pingLanServers(SocketUDP &socketUDP, AddressType type) {
    m_pingedServers.clear();
    for (uint16_t port = DEFAULT_PORT; port < DEFAULT_PORT + MAX_TRY_PORTS; port++) {
        auto message = UDPMessage(0, CL_BROADCAST_PING);

        if (type == AT_IPV4)
            socketUDP.send(message, AddressV4(AT_BROADCAST, port));
        else if (type == AT_IPV6)
            socketUDP.send(message, AddressV6(AT_MULTICAST, port));
    }
}

} // namespace Network
