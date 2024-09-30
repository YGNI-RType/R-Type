/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet (Workspace)
** File description:
** msg
*/

#include "GEngine/net/msg.hpp"

namespace Network {
UDPMessage::UDPMessage(std::size_t maxSize, uint8_t type) : AMessage(maxSize, type) {}

UDPMessage &UDPMessage::operator=(const UDPMessage &other) {
    m_isCompressed = other.m_isCompressed;
    fragments = other.fragments;
    m_curSize = other.m_curSize;
    m_type = other.m_type;
    std::memcpy(m_data, other.m_data, m_curSize);
    return *this;
}

void UDPMessage::writeData(const void *data, std::size_t size) {
    std::memcpy(m_data + m_curSize, data, size);
    m_curSize += size;
}

// void UDPMessage::writeHeader() {
//     // put the time
//     // put the mode
// }

// void UDPMessage::readHeader() {
//     // get the time
// }

} // namespace Network
