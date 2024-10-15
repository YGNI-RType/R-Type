/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Bullet.hpp
*/

#pragma once

#include "GEngine/libdev/Component.hpp"

#include "boost/uuid/uuid.hpp"

namespace rtype::component {
struct Bullet : public gengine::Component<Bullet> {
    Bullet(std::string fromString, bool isBeam = false)
        : isBeam(isBeam) {
        for (size_t i = 0; i < 36 && i < fromString.size(); ++i)
            from[i] = static_cast<unsigned char>(fromString[i]);
    }
    unsigned char from[36];
    bool isBeam = false;
};
} // namespace rtype::component
