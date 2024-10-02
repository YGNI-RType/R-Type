/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Drawable.hpp
*/

#pragma once

#include <cstdlib>

#include "GEngine/libdev/Component.hpp"

namespace gengine::component::driver::output {
struct Drawable : public gengine::Component {
    std::size_t zIndex;
    Drawable(std::size_t zIndex) : zIndex(zIndex) {
    }
};
} // namespace gengine::component::driver::output