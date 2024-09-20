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

        template <typename ...Components>
        Entity spawnEntity(Components&& ...components)
        {
            Entity entity = createEntity();
            (setComponent(entity, std::forward<Components>(components)), ...);
            return entity;
        }

        void killEntity(Entity entity);
    };
} // namespace ecs
