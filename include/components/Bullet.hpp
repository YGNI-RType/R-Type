/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Bullet.hpp
*/

#pragma once

#include "GEngine/libdev/Component.hpp"

namespace rtype::component {
struct Bullet : public gengine::Component<Bullet> {
    Bullet(bool isBeam = false)
        : isBeam(isBeam) {
    }
    bool isBeam = false;
};
} // namespace rtype::component
