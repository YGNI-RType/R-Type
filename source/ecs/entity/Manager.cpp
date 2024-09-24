/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** entity::Manager.cpp
*/

#include "ecs/entity/Manager.hpp"

using namespace ecs;

std::vector<entity::Entity> entity::Manager::m_entities = {};

entity::Manager::Manager() {}

entity::Entity entity::Manager::createEntity() {
    entity::Entity entity = m_entities.size();
    m_entities.push_back(entity);
    return entity;
}

void entity::Manager::destroyEntity(entity::Entity dead) {
    std::size_t lastIndex = m_entities.size() - 1;
    for (auto entity : m_entities) {
        if (entity == dead) {
            entity = m_entities[lastIndex];
            break;
        }
    }
    m_entities.pop_back();
}
