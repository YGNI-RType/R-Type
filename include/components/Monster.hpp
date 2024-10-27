/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Monster.hpp
*/

#pragma once

#include "GEngine/libdev/Component.hpp"

namespace rtype::component {
struct Monster : public gengine::Component<Monster> {
    Monster() {
    }
};

} // namespace rtype::component
