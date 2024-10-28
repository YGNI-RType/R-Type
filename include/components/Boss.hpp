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
    Boss(float ballSpeed = 6, float maxVelocity = 10, float borderMargin = 150, float waveSpawnCooldown = 5,
         size_t wavePlanesCount = 10)
        : ballSpeed(ballSpeed)
        , maxVelocity(maxVelocity)
        , borderMargin(borderMargin)
        , waveSpawnCooldown(waveSpawnCooldown)
        , wavePlanesCount(wavePlanesCount) {
    }
    float ballSpeed;
    float maxVelocity;
    float borderMargin;

    float waveSpawnInSec;
    float waveSpawnCooldown;
    size_t wavePlanesCount;
};
} // namespace rtype::component
