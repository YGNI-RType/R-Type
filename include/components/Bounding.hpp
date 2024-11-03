/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Bounding.hpp
*/

#pragma once

#include "GEngine/libdev/Component.hpp"

namespace rtype::component {
struct Bounding : public gengine::Component<Bounding> {
    Bounding() {
    }

    bool operator==(const Bounding &) const = default;
};
} // namespace rtype::component
