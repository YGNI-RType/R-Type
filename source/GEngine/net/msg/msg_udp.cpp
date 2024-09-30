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

void UDPMessage::readData(void *data, std::size_t size) const {
    if (size > m_curSize)
        throw std::runtime_error("Not enough data to read");
    std::memcpy(data, m_data + m_curSize, size);
}

void UDPMessage::setSerialize(UDPSerializedMessage &msg) {
    m_type = msg.m_type;
    m_curSize = msg.m_curSize;
    m_isCompressed = msg.m_isCompressed;
    fragments = msg.fragments;
    std::memcpy(m_data, &msg.data, m_curSize);
}

void UDPMessage::getSerialize(UDPSerializedMessage &msg) const {
    msg.m_type = m_type;
    msg.m_curSize = m_curSize;
    msg.m_isCompressed = m_isCompressed;
    msg.fragments = fragments;
    std::memcpy(&msg.data, m_data, m_curSize);
}

// void UDPMessage::writeHeader() {
//     // put the time
//     // put the mode
// }

// void UDPMessage::readHeader() {
//     // get the time
// }

} // namespace Network
