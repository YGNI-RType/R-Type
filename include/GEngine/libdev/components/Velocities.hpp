/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Velocities.hpp
*/

#pragma once

#include "GEngine/libdev/Component.hpp"

namespace gengine::component {
struct Velocity2D : public Component {
    float x, y;
    Velocity2D(float x, float y)
        : x(x)
        , y(y) {
    }
};

struct Velocity3D : public Component {
    float x, y, z;
    Velocity3D(float x, float y, float z)
        : x(x)
        , y(y)
        , z(z) {
    }
};
} // namespace gengine::component
