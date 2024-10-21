/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** EnemyShootEvent.hpp
*/

#pragma once

#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/Entity.hpp"

namespace rtype::event {
struct EnemyShootEvent : public gengine::Event {
    gengine::Entity from;

    EnemyShootEvent(size_t from)
        : from(from) {
    }
};
} // namespace rtype::event
