/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** net_server
*/

#include "GEngine/net/net_server.hpp"
#include "GEngine/cvar/net.hpp"

namespace Network {

uint16_t NetServer::start(size_t maxClients, const std::vector<IP> &g_localIPs, uint16_t currentUnusedPort) {
    // TODO : cloes everything if already initted
    if (m_isRunning)
        return currentUnusedPort;

    for (const IP &ip : g_localIPs) { // todo : force an ip, find the best ip
                                      // (privileging pubilc interface)
        if (ip.type == AT_IPV4) {
            m_socketv4 = openSocketTcp(ip, currentUnusedPort);
            currentUnusedPort++;
        }
        if (ip.type == AT_IPV6 && CVar::net_ipv6.getIntValue()) { // check if ipv6 is supported
            m_socketv6 = openSocketTcp(ip, currentUnusedPort);
            currentUnusedPort++;
        }
        break;
    }

    m_isRunning = true;
    return currentUnusedPort;
}

void NetServer::stop(void) {
    m_clients.clear();
    m_isRunning = false;
}

void NetServer::createSets(fd_set &readSet) {
    if (!isRunning())
        return;

    for (const auto &client : m_clients) {
        auto &socket = client->getChannel().getTcpSocket();
        auto eventType = socket.getEventType();
        auto socketId = socket.getSocket();

        if (eventType == SocketTCP::EventType::READ)
            FD_SET(socketId, &readSet);
    }

    FD_SET(m_socketv4.getSocket(), &readSet);
    if (CVar::net_ipv6.getIntValue())
        FD_SET(m_socketv6.getSocket(), &readSet);
}

void NetServer::respondPingServers(const UDPMessage &msg, SocketUDP &udpsocket, const Address &addr) {
    if (!isRunning())
        return;

    auto pingReponseMsg = UDPMessage(0, SV_BROADCAST_PING);
    UDPSV_PingResponse data = {.tcpv4Port = m_socketv4.getPort(),
                               .tcpv6Port = m_socketv6.getPort(),
                               .maxPlayers = getMaxClients(),
                               .currentPlayers = getNumClients()};

    pingReponseMsg.writeData<UDPSV_PingResponse>(data);
    udpsocket.send(pingReponseMsg, addr);
}

void NetServer::handleNewClient(SocketTCPMaster &socket) {
    UnknownAddress unkwAddr; 
    auto newSocket = socket.accept(unkwAddr);

    if (getNumClients() >= getMaxClients()) {
        newSocket.socketClose();
        return;
    }
    
    if (unkwAddr.getType() == AT_IPV4)
        m_clients.push_back(std::move(std::make_unique<NetClient>(std::make_unique<AddressV4>(unkwAddr.getV4()), newSocket)));
    else if(unkwAddr.getType() == AT_IPV6) 
        m_clients.push_back(std::move(std::make_unique<NetClient>(std::make_unique<AddressV6>(unkwAddr.getV6()), newSocket)));
}

bool NetServer::handleUDPEvent(SocketUDP &socket, const UDPMessage &msg, const Address &addr)
{
    switch (msg.getType()) {
    case CL_BROADCAST_PING:
        respondPingServers(msg, socket, addr);
        return true;
    default: // handleUdpMessage(msg, addr);
        return false;
    }
}

bool NetServer::handleTCPEvent(fd_set &readSet) {
    if (!isRunning())
        return false;

    if (FD_ISSET(m_socketv4.getSocket(), &readSet)) {
        handleNewClient(m_socketv4);
        return true;
    }

    if (CVar::net_ipv6.getIntValue() && FD_ISSET(m_socketv6.getSocket(), &readSet)) {
        handleNewClient(m_socketv6);
        return true;
    }

    for (const auto &client : m_clients) {
        auto &socket = client->getChannel().getTcpSocket();
        auto eventType = socket.getEventType();
        auto socketId = socket.getSocket();

        if (eventType == SocketTCP::EventType::READ && FD_ISSET(socketId, &readSet))
        //     socket.handleEvent();
            return true;
    }

    return false;
}

} // namespace Network
