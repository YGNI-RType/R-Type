/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Player.hpp
*/

#pragma once

#include "GEngine/libdev/Component.hpp"
#include <string>

#define DEFAULT_SPEED 500
#define DEFAULT_ATTACK_SPEED 3 // Shoot per second

namespace rtype::component {
struct Player : public gengine::Component<Player> {
    char name[32];
    float speed;
    float attackSpeed;

    Player(const std::string &newName, float newSpeed = DEFAULT_SPEED, float newAttackSpeed = DEFAULT_ATTACK_SPEED)
        : speed(newSpeed)
        , attackSpeed(newAttackSpeed) {
        strcpy(name, newName.c_str());
    }
};

} // namespace rtype::component
