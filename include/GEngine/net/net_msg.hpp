/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** In contrary of normal messages, the message are stored here for sending and receiving and after transformed to
** appropriate messages. It's working with netchannels to maximize efficiency and rate values
*/

#pragma once

#include <cstdint>

namespace Network
{
    /* each NetChannel has one */
    struct iomsg {
        std::size_t pos_start;
        std::size_t pos_end;
    };
} // namespace Network
