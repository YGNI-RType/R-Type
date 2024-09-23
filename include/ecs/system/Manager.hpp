/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** SystemManager.hpp
*/

#pragma once

#include "ecs/system/event/Bus.hpp"
#include "ecs/system/Base.hpp"
#include <typeindex>
#include <unordered_map>
#include <optional>
#include <functional>
#include <memory>
#include <any>

namespace ecs {
    class ECS;
};

namespace ecs::system {
    class Manager {
    public:
        Manager(ECS &ecs): m_ecs(ecs), m_eventBus() {}

        template <class T, class ...Params>
        void registerSystem(Params &&...p) {
            m_systemTable[std::type_index(typeid(T))] = std::make_any<T>(std::forward<Params>(p)...);
            T &system = (std::any_cast<T&>(m_systemTable[std::type_index(typeid(T))]));
            system.m_eventBus = m_eventBus;
            system.m_ecs = m_ecs;
            system.init();
        }

        template<class T>
        T &getSystem(void) {
            auto it = m_systemTable.find(std::type_index(typeid(T)));

            if (it == m_systemTable.end())
                throw "";
            return std::any_cast<T&>(it->second);
        }

        template<class T>
        void publishEvent(T &event) {
            m_eventBus.publish<T>(event);
        }

        template<class T>
        void publishEvent(T &&event) {
            m_eventBus.publish<T>(event);
        }

    private:
        std::unordered_map<std::type_index, std::any> m_systemTable;
        event::Bus m_eventBus;
        ECS &m_ecs;
    };
}
