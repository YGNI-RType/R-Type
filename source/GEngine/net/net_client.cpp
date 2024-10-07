/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** net_client
*/

#include "GEngine/net/net_client.hpp"

namespace Network {
NetClient::NetClient(std::unique_ptr<Address> addr, SocketTCP &&socket, SocketUDP &socketudp)
    : m_channel(true, std::move(addr), std::move(socket)), m_socketUdp(socketudp) {
}
} // namespace Network
