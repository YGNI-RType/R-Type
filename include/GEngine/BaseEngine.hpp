/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** AEngine.hpp
*/

#pragma once

#include "GEngine/interface/Base.hpp"
#include "ecs/ECS.hpp"
#include <memory>
#include <functional>

namespace gengine {
    class BaseEngine {
    public:
        // TODO add constructor whit Interface Type template
        template<typename T, typename ...Params>
        inline void registerSystem(Params &&...p) {
            m_ecs.registerSystem<T>(std::forward<Params>(p)...);
        }

        template<typename T>
        inline void registerComponent(void) {
            m_ecs.registerComponent<T>();
        }

        void update(void) {
            m_ecs.update();
        }

        void start(void) {
            m_ecs.start();
        }

        // void start

    private:
        ecs::ECS m_ecs;
    };
}
