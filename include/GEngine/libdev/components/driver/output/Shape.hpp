/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Shape.hpp
*/

#pragma once

#include <raylib.h>

#include "GEngine/libdev/Component.hpp"

namespace gengine::component::driver::output {
struct Rectangle : public gengine::Component {
    int x, y, width, height;

    Rectangle(int x, int y, int width, int height) : x(x), y(y), width(width), height(height){};
    // Rectangle(int x, int y, int width, int height) : x(x), y(y), width(width), height(height) {};
};
} // namespace gengine::component::driver::output
