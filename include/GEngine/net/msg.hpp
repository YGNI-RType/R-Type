/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet (Workspace)
** File description:
** msg
*/

#pragma once

#include <cstdint>
#include "structs/msg_tcp_structs.hpp"
#include "structs/msg_udp_structs.hpp"

namespace Network {

typedef uint8_t byte_t;

#define MAX_MSGLEN 16384

class AMessage {
  public:
    /* there is no constructor, the data is casted as the class it was meant to
     * be delivered */
    /* commented since we don't use AMessage anyway, so don't use the vtable */
    // virtual void writeHeader(); // override final;
    // virtual void readHeader(); // override final;




  protected:
    AMessage(std::size_t maxSize, uint8_t type);
    ~AMessage() = default;

    const std::size_t m_maxSize;
    std::size_t m_curSize = 0;
    uint8_t m_type;

    byte_t m_data[MAX_MSGLEN];
};

class TCPMessage : public AMessage {
  public:
    TCPMessage(std::size_t maxSize, uint8_t type);

    void writeHeader(); // override final;
    void readHeader(); // override final;

  private:
    bool m_isFinished = false;
};

class UDPMessage : public AMessage {
  public:
    UDPMessage(std::size_t maxSize, uint8_t type);

    void writeHeader(); // override final;
    void readHeader(); // override final;

  private:
    bool m_isCompressed = false;
    uint16_t fragments = 0;
};

} // namespace Network
