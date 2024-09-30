/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** net_channel
*/

#include "GEngine/net/net_channel.hpp"

namespace Network {

NetChannel::NetChannel(std::unique_ptr<Address> clientAddress, SocketTCP &socket)
    : m_toAddress(std::move(clientAddress)), m_tcpSocket(std::move(socket)) {}
} // namespace Network