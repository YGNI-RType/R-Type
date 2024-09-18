#pragma once

#include "ComponentManager.hpp"
#include "EntityManager.hpp"
#include <memory>

namespace ecs {
    class Core {
    public:
        Core();

        template <typename Component>
        void registerComponent();

        Entity createEntity();

        template <typename Component>
        void setComponent(Entity entity, Component component)
        {
            m_componentManager->setComponent<Component>(entity, component);
        }

        template <typename Component, class ...Params>
        void setComponent(Entity entity, Params &&...p)
        {
            m_componentManager->setComponent<Component>(entity, std::forward<Params>(p)...);
        }

    private:
        std::unique_ptr<ComponentManager> m_componentManager;
        std::unique_ptr<EntityManager> m_entityManager;
    };
} // namespace ecs
