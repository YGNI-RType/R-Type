#pragma once

#include <any>
#include <typeindex>
#include <unordered_map>
#include <functional>

#include "Entity.hpp"
#include "ComponentSparseArray.hpp"

#include <string>
#include <stdexcept>
#include <iostream>

namespace ecs {
    class ComponentManager {
    public:
        ComponentManager() {};

        template <class Component>
        ComponentSparseArray<Component>& registerComponent()
        {
            auto res = m_componentArrays.emplace(std::type_index(typeid(Component)), std::make_pair(ComponentSparseArray<Component>(),
                [this](Entity entity) {
                    destroyComponent<Component>(entity);
                }));
            return std::any_cast<ComponentSparseArray<Component>&>(res.first->second.first);
        }

        template <class Component>
        void setComponent(Entity entity, const Component& component)
        {
            getComponents<Component>().insert(entity, component);
        }

        void destroyComponents(Entity entity)
        {
            for (auto& [_, pair] : m_componentArrays)
                pair.second(entity);
        }

        template <class Component>
        void destroyComponent(Entity entity)
        {
            getComponents<Component>().erase(entity);
        }

        template <typename Component, class ...Params>
        void setComponent(Entity entity, Params &&...p)
        {
            getComponents<Component>().emplace(entity, Component(std::forward<Params>(p)...));
        }

        template<class Component>
        ComponentSparseArray<Component>& getComponents()
        {
            auto it = m_componentArrays.find(std::type_index(typeid(Component)));
            if (it == m_componentArrays.end())
                throw std::runtime_error("The component " + std::string(typeid(Component).name()) + " does not exist in the Manager");
            return std::any_cast<ComponentSparseArray<Component>&>(it->second.first);
        }

        template<class Component>
        const ComponentSparseArray<Component>& getComponents() const
        {
            auto it = m_componentArrays.find(std::type_index(typeid(Component)));
            if (it == m_componentArrays.end())
                throw std::runtime_error("The component " + std::string(typeid(Component).name()) + " does not exist in the Manager");
            return std::any_cast<const ComponentSparseArray<Component>&>(it->second);
        }

    private:
        typedef std::function<void(Entity)> destroyer_t;
        std::unordered_map<std::type_index, std::pair<std::any, destroyer_t>> m_componentArrays;
    };

} // namespace ecs
