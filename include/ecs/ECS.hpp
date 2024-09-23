/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Core.hpp
*/

#pragma once

#include "ecs/entity/Manager.hpp"
#include "ecs/component/Manager.hpp"
#include "ecs/system/Manager.hpp"
#include "ecs/system/Base.hpp"
#include "ecs/system/event/Base.hpp"
#include <memory>

namespace ecs {
    class ECS : public component::Manager, private entity::Manager, public system::Manager {
    public:
        ECS();

        template <typename ...Components>
        entity::Entity spawnEntity(Components&& ...components)
        {
            entity::Entity entity = createEntity();
            (setComponent(entity, std::forward<Components>(components)), ...);
            return entity;
        }

        void start(void) {
            publishEvent(ecs::system::event::StartEngine());
        }

        void update(void) {
            publishEvent(ecs::system::event::MainLoop());
        }

        void killEntity(entity::Entity entity);
    };
}
