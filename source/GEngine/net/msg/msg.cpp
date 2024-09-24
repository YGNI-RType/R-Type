/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet (Workspace)
** File description:
** msg
*/

#include "GEngine/net/msg.hpp"

namespace Network {
    AMessage::AMessage(std::size_t maxSize, uint8_t type) : m_maxSize(maxSize), m_type(type) {}

} // namespace Network
