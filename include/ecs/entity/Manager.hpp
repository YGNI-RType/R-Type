/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Manager.hpp
*/

#pragma once

#include "ecs/entity/Entity.hpp"
#include <queue>

namespace ecs::entity {
class Manager {
public:
    Manager();

        Entity createEntity();
        void destroyEntity(Entity dead);
    private:
        std::size_t m_size;
        std::queue<Entity> m_available;
    };
}
