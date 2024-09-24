/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** ECS.cpp
*/

#include "ecs/ECS.hpp"
#include <iostream>

using namespace ecs;

ECS::ECS() : ecs::system::Manager(*this) {}

void ECS::killEntity(entity::Entity entity) {
    destroyComponents(entity);
    destroyEntity(entity);
}
