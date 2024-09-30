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
#include <cstring>
#include <stdexcept>

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

    TCPMessage &operator=(const TCPMessage &other);

    const byte_t *getData() const { return m_data; }

private:
    bool m_isFinished = true;

    /* always set field to last, this is not a header !!!*/
    byte_t m_data[MAX_TCP_MSGLEN];
};

/* TODO : make a pack for MSVC */
struct __attribute__((__packed__)) UDPSerializedMessage {
    uint8_t m_type;
    std::size_t m_curSize;
    bool m_isCompressed;
    uint16_t fragments;
    byte_t data[MAX_UDP_MSGLEN];
};

class UDPMessage : public AMessage {
public:
    UDPMessage(std::size_t maxSize, uint8_t type);

    UDPMessage &operator=(const UDPMessage &other);

    const byte_t *getData() const { return m_data; }
    void getSerialize(UDPSerializedMessage &msg) const;
    void setSerialize(UDPSerializedMessage &msg);

    template <typename T> void writeData(const T &data) {
        std::memcpy(m_data + m_curSize, &data, sizeof(T));
        m_curSize += sizeof(T);
    }

    template <typename T> size_t readData(T &data) const {
        if (sizeof(T) > m_curSize)
            throw std::runtime_error("Message is too small to read data");
        std::memcpy(&data, m_data, sizeof(T));
        return sizeof(T);
    }

    void writeData(const void *data, std::size_t size);
    void readData(void *data, std::size_t size) const;

private:
    bool m_isCompressed = false;
    uint16_t fragments = 0;

    /* always set field to last, this is not a header !!!*/
    byte_t m_data[MAX_UDP_MSGLEN];
};

} // namespace Network
