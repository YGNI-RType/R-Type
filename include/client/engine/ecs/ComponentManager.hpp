#pragma once

#include "ComponentSparseArray.hpp"
// #include <map>
#include <any>
#include <typeindex>
#include <unordered_map>

namespace ecs {
    class ComponentManager {
    public:
        ComponentManager() {};

        template <typename Component>
        void registerComponent()
        {
            m_componentArrays.push_back(std::make_shared<ComponentSparseArray<Component>>());
        }

        template <typename Component>
        void setComponent(Entity entity, const Component& component)
        {
            m_componentArrays[std::type_index(typeid(Component))]->setComponent(entity, component)
        }
        template <typename Component, class ...Params>
        void setComponent(Entity entity, Params &&...p)
        {
            setComponent(entity, Component(std::forward<Params>(p)...))
        }

    private:
        std::unordered_map<std::type_index, std::any> m_componentArrays;
    };

} // namespace ecs
