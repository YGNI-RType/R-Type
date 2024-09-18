#pragma once

#include "Entity.hpp"

namespace ecs {
    class EntityManager {
    public:
        EntityManager();

        Entity createEntity();
    private:
        std::vector<Entity> m_entities{};
    };
}
