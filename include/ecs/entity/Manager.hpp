/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Manager.hpp
*/

#pragma once

#include "ecs/entity/Entity.hpp"
#include <vector>

namespace ecs::entity {
class Manager {
public:
    Manager();

    Entity createEntity();
    void destroyEntity(Entity dead);

private:
    static std::vector<Entity> m_entities;
};
} // namespace ecs::entity
