/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** EntityManager.hpp
*/

#pragma once

#include "Entity.hpp"
#include <vector>

namespace ecs {
    class EntityManager {
    public:
        EntityManager();

        Entity createEntity();
        void destroyEntity(Entity dead);
    private:
        static std::vector<Entity> m_entities;
    };
}
