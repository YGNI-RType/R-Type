/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Core.hpp
*/

#pragma once

#include "ComponentManager.hpp"
#include "EntityManager.hpp"
#include <memory>

namespace ecs {
    class Core : public ComponentManager, public EntityManager {
    public:
        Core();

        // template <typename Component>
        // void registerComponent();

        template <typename ...Components>
        Entity spawnEntity(Components&& ...components) {
        // Create a new entity
        Entity entity = createEntity();

        // Set each component for the newly created entity
        (setComponent(entity, std::forward<Components>(components)), ...);

        return entity;
    }

        // template <class ...Component, typename ...Params>
        // Entity spawnEntity(const Params&... c)
        // {
        //     Entity entity = createEntity();

        //     (addComponent(entity, c), ...);

        //     return entity;
        // }

        void deleteEntity(Entity entity);

        // template <typename Component>
        // void setComponent(Entity entity, Component component)
        // {
        //     m_componentManager->setComponent<Component>(entity, component);
        // }

        // template <typename Component, class ...Params>
        // void setComponent(Entity entity, Params &&...p)
        // {
        //     m_componentManager->setComponent<Component>(entity, std::forward<Params>(p)...);
        // }
    };
} // namespace ecs
