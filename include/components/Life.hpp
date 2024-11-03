/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Life.hpp
*/

#pragma once

#include "GEngine/libdev/Component.hpp"

namespace rtype::component {
struct Life : public gengine::Component<Life> {
    Life() {
    }
    bool operator==(const Life &) const = default;
};
} // namespace rtype::component
