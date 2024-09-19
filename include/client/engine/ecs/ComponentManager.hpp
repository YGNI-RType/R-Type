#pragma once

#include "Entity.hpp"
#include "ComponentSparseArray.hpp"
// #include <map>
#include <any>
#include <typeindex>
#include <unordered_map>

#include <string>
#include <stdexcept>

namespace ecs {
    class ComponentManager {
    public:
        ComponentManager() {};

        template <class Component>
        ComponentSparseArray<Component>& registerComponent()
        {
            auto res = m_componentArrays.emplace(std::type_index(typeid(Component)), std::any(ComponentSparseArray<Component>()));
            return std::any_cast<ComponentSparseArray<Component>&>(res.first->second);
        }

        template <class Component>
        void setComponent(Entity entity, const Component& component)
        {
            getComponents<Component>().insert(entity, component);
        }

        template <class Component>
        void eraseComponent(Entity entity)
        {
            getComponents<Component>().erase(entity);
        }

        template <typename Component, class ...Params>
        void setComponent(Entity entity, Params &&...p)
        {
            getComponents<Component>().emplace(entity, Component(std::forward<Params>(p)...));
        }

        template<class Component>
        ComponentSparseArray<Component> &getComponents()
        {
            auto it = m_componentArrays.find(std::type_index(typeid(Component)));
            if (it == m_componentArrays.end())
                throw std::runtime_error("The component " + std::string(typeid(Component).name()) + " does not exist in the Manager");
            return std::any_cast<ComponentSparseArray<Component>&>(it->second);
        }

        template<class Component>
        const ComponentSparseArray<Component> &getComponents() const
        {
            auto it = m_componentArrays.find(std::type_index(typeid(Component)));
            if (it == m_componentArrays.end())
                throw std::runtime_error("The component " + std::string(typeid(Component).name()) + " does not exist in the Manager");
            return std::any_cast<const ComponentSparseArray<Component>&>(it->second);
        }

    protected:
        std::unordered_map<std::type_index, std::any> m_componentArrays;
    };

} // namespace ecs
