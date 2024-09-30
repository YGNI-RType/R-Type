/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet (Workspace)
** File description:
** msg
*/

#include "GEngine/net/msg.hpp"

namespace Network {

TCPMessage::TCPMessage(std::size_t maxSize, uint8_t type) : AMessage(maxSize, type) {}

TCPMessage &TCPMessage::operator=(const TCPMessage &other) {
    m_isFinished = other.m_isFinished;
    m_curSize = other.m_curSize;
    m_type = other.m_type;
    std::memcpy(m_data, other.m_data, m_curSize);
    return *this;
}

} // namespace Network
