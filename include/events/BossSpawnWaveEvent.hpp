/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** BossSpawnWaveEvent.hpp
*/

#pragma once

#include "GEngine/libdev/Entity.hpp"
#include "GEngine/libdev/System.hpp"

namespace rtype::event {
struct BossSpawnWaveEvent : public gengine::Event {
    gengine::Entity from;
    size_t planeCount;

    BossSpawnWaveEvent(size_t from, size_t planeCount)
        : from(from)
        , planeCount(planeCount) {
    }
};
} // namespace rtype::event
