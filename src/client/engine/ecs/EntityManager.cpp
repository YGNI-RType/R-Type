#include "EntityManager.hpp"

namespace ecs {
    EntityManager::EntityManager()
    {
        // m_entities.resize(MAX_ENTITIES);
    }

    Entity EntityManager::createEntity()
    {
        Entity entity = m_entities.size();
        m_entities.push_back(entity);
        return entity;
    }
}
