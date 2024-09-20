/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** EntityManager.cpp
*/

#include "EntityManager.hpp"

using namespace ecs;

std::vector<Entity> EntityManager::m_entities = {};

EntityManager::EntityManager()
{
}

Entity EntityManager::createEntity()
{
    Entity entity = m_entities.size();
    m_entities.push_back(entity);
    return entity;
}

void EntityManager::destroyEntity(Entity dead)
{
    std::size_t lastIndex = m_entities.size() - 1;
    for (auto entity : m_entities) {
        if (entity == dead) {
            entity = m_entities[lastIndex];
            break;
        }
    }
    m_entities.pop_back();
}
