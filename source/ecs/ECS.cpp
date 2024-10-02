/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** ECS.cpp
*/

#include "ecs/ECS.hpp"

using namespace ecs;

ECS::ECS() : system::Manager(*this) {
}

void ECS::killEntity(entity::Entity entity) {
    destroyComponents(entity);
    destroyEntity(entity);
}

void ECS::start(void) {
    publishEvent(ecs::system::event::StartEngine());
}

void ECS::update(void) {
    publishEvent(ecs::system::event::MainLoop());
}