/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet (Workspace)
** File description:
** msg
*/

#include "GEngine/net/msg.hpp"
#include "GEngine/net/socket.hpp"

namespace Network {
UDPMessage::UDPMessage(bool hasHeader, uint8_t type) : AMessage(0, type)
{
    m_curSize = hasHeader ? sizeof(UDPG_NetChannelHeader) : 0;
}

UDPMessage &UDPMessage::operator=(const UDPMessage &other) {
    m_flags = other.m_flags;
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
    m_type = msg.type;
    m_flags = msg.flag;
    std::memcpy(m_data + m_curSize, &msg.data, msg.curSize);
    m_curSize += msg.curSize;
}

void UDPMessage::getSerialize(UDPSerializedMessage &msg) const {
    msg.type = m_type;
    msg.curSize = m_curSize;
    msg.flag = m_flags;
    msg.fragments = 0;
    std::memcpy(&msg.data, m_data, m_curSize);
}

std::vector<UDPSerializedMessage> UDPMessage::getSerializeFragmented(void) const {

    uint32_t nbFragments = m_curSize / FRAGMENT_SIZE;
    std::vector<UDPSerializedMessage> fragments(nbFragments);

    for (uint32_t i = 0; i < nbFragments; i++) {
        fragments[i].type = m_type;
        fragments[i].curSize = FRAGMENT_SIZE;
        fragments[i].flag = m_flags;
        fragments[i].fragments = i | FRAGMENT_BIT;
        std::memcpy(&fragments[i].data, m_data + i * FRAGMENT_SIZE, FRAGMENT_SIZE);
    }

    return fragments;
}

void UDPMessage::writeHeader(const UDPG_NetChannelHeader &header)
{
    /* bypasses the m_cursize, since normally, it should contain this exact amount of data inside */
    std::memcpy(m_data, &header, sizeof(UDPG_NetChannelHeader));
}

void UDPMessage::readHeader(UDPG_NetChannelHeader &header) const
{
    std::memcpy(&header, m_data, sizeof(UDPG_NetChannelHeader));
}

void UDPMessage::setCompressed(bool compressed)
{
    if (compressed) {
        m_flags |= COMPRESSED;
    } else {
        m_flags &= ~COMPRESSED;
    }
}

void UDPMessage::setHeader(bool header)
{
    if (header) {
        m_flags |= HEADER;
    } else {
        m_flags &= ~HEADER;
    }
}

void UDPMessage::setFragmented(bool fragmented)
{
    if (fragmented) {
        m_flags |= FRAGMENTED;
    } else {
        m_flags &= ~FRAGMENTED;
    }
}

void UDPMessage::setEncrypted(bool encrypted)
{
    if (encrypted) {
        m_flags |= ENCRYPTED;
    } else {
        m_flags &= ~ENCRYPTED;
    }
}


} // namespace Network
