/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Boss.hpp
*/

#pragma once

#include "GEngine/libdev/Component.hpp"

namespace rtype::component {
struct Boss : public gengine::Component<Boss> {
    Boss(const std::string &bossName_, unsigned int startLives_)
        : bossName(bossName_)
        , startLives(startLives_)
        , waveSpawnInSec(0) {
    }

    Network::NetString<32> bossName;
    unsigned int startLives;
    int waveSpawnInSec;

    bool operator==(const Boss &) const = default;
};
} // namespace rtype::component
