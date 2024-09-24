/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet (Workspace)
** File description:
** msg
*/

#include "GEngine/net/msg.hpp"

namespace Network {

TCPMessage::TCPMessage(std::size_t maxSize, uint8_t type) : AMessage(maxSize, type) {}
} // namespace Network
