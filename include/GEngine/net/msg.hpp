/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet (Workspace)
** File description:
** msg
*/

#pragma once

#include "net_common.hpp"
#include "structs/msg_tcp_structs.hpp"
#include "structs/msg_udp_structs.hpp"
#include <cstddef>
#include <cstdint>

namespace Network {

class AMessage {
public:
    /* there is no constructor, the data is casted as the class it was meant to
     * be delivered */
    /* commented since we don't use AMessage anyway, so don't use the vtable */
    // virtual void writeHeader(); // override final;
    // virtual void readHeader(); // override final;

    std::size_t getSize() const { return m_curSize; }
    uint8_t getType() const { return m_type; }
    std::size_t getMaxSize() const { return m_maxSize; }

protected:
    AMessage(std::size_t maxSize, uint8_t type);
    ~AMessage() = default;

    const std::size_t m_maxSize;
    std::size_t m_curSize = 0;
    uint8_t m_type;
};

class TCPMessage : public AMessage {
public:
    TCPMessage(std::size_t maxSize, uint8_t type);

    void writeHeader(); // override final;
    void readHeader();  // override final;
    const byte_t *getData() const { return m_data; }

private:
    bool m_isFinished = true;

    /* always set field to last, this is not a header !!!*/
    byte_t m_data[MAX_TCP_MSGLEN];
};

class UDPMessage : public AMessage {
public:
    UDPMessage(std::size_t maxSize, uint8_t type);

    void writeHeader(); // override final;
    void readHeader();  // override final;
    const byte_t *getData() const { return m_data; }

private:
    bool m_isCompressed = false;
    uint16_t fragments = 0;

    /* always set field to last, this is not a header !!!*/
    byte_t m_data[MAX_UDP_MSGLEN];
};

} // namespace Network
