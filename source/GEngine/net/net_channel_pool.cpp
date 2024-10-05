/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** net_channel_pool
*/

#include "GEngine/net/net_channel.hpp"

namespace Network {
bool PacketPoolUdp::addMessage(uint32_t sequence, const UDPMessage &msg) {
    auto size = msg.getSize() / MAX_UDP_PACKET_LENGTH;
    auto remsize = msg.getSize() % MAX_UDP_PACKET_LENGTH;
    if (size > FRAG_SEQUENCE_TABLE_SZ)
        return false;

    m_poolSequences[sequence] =
        std::make_tuple<>(msg.getType(), static_cast<uint8_t>(size), static_cast<uint16_t>(remsize), m_pool.size());

    auto data = msg.getData();
    if (msg.hasHeader())
        data += sizeof(UDPG_NetChannelHeader);

    for (uint8_t i = 0; i < size; i++) {
        chunk_t chunk;
        std::memcpy(chunk.data(), data + i * CHUNK_SIZE, CHUNK_SIZE);
        m_pool.push_back(chunk);
    }

    chunk_t chunk;
    std::memcpy(chunk.data(), data + size * CHUNK_SIZE, remsize);
    m_pool.push_back(chunk);
    return true;
}

std::vector<const PacketPoolUdp::chunk_t *> PacketPoolUdp::getMissingFragments(uint32_t sequence, uint16_t mask) {
    auto it = m_poolSequences.find(sequence);
    if (it == m_poolSequences.end())
        return {};

    auto [type, size, remsize, offset] = it->second;
    std::vector<const chunk_t *> fragments;

    for (uint8_t i = 0; i < size; i++)
        if (!(mask & (1 << i)))
            fragments.push_back(&m_pool[offset + i]);
    if (!(mask & (1 << size)))
        fragments.push_back(&m_pool[offset + size]);
    return fragments;
}

void PacketPoolUdp::constructMessage(UDPMessage &msg, const chunk_t *chunk, size_t chunk_size, const UDPG_FragmentHeaderTo &header) {
    msg.appendData<UDPG_FragmentHeaderTo>(header);
    msg.appendData(chunk->data(), chunk_size);
}
} // namespace Network
