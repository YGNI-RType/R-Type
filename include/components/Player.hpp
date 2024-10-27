/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Player.hpp
*/

#pragma once

#include "GEngine/libdev/Component.hpp"

namespace rtype::component {
struct Player : public gengine::Component<Player> {
    int life = 3;

    Player() {
    }
};
} // namespace rtype::component
