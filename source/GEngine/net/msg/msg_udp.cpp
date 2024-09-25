/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet (Workspace)
** File description:
** msg
*/

#include "GEngine/net/msg.hpp"

namespace Network {
UDPMessage::UDPMessage(std::size_t maxSize, uint8_t type) : AMessage(maxSize, type) {}

void UDPMessage::writeHeader() {
    // put the time
    // put the mode
}

void UDPMessage::readHeader() {
    // get the time
}

} // namespace Network
