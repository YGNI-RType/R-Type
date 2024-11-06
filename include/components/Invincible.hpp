/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Invincible.hpp
*/

#pragma once

#include "GEngine/libdev/Component.hpp"

namespace rtype::component {
struct Invincible : public gengine::Component<Invincible> {
    Invincible(int duration_ = 2000)
        : duration(duration_) {
    }

    int duration;
    bool operator==(const Invincible &) const = default;
};
} // namespace rtype::component
