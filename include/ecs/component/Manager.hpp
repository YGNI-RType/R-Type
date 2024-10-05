/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** component::Manager.hpp
*/

#pragma once

#include <any>
#include <cstring>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <typeindex>
#include <unordered_map>

#include "ecs/component/IsComponent.hpp"
#include "ecs/component/Iterator.hpp"
#include "ecs/component/SparseArray.hpp"
#include "exceptions/Base.hpp"

namespace ecs::component {
class Manager {
public:
    typedef std::function<void(entity::Entity)> destroyer_t;
    typedef std::function<void(entity::Entity, const std::any &)> setter_t;
    typedef std::function<std::vector<component_info_t>(const std::any &, const std::any &)> comparer_t;

    using component_tools_t = std::tuple<destroyer_t, setter_t, comparer_t>;
    using component_map_t = std::unordered_map<std::type_index, std::pair<std::any, component_tools_t>>;
    Manager(void);

    template <class Component>
    SparseArray<Component> &registerComponent(void);

    template <class Component>
    void setComponent(entity::Entity from, const Component &component);
    template <typename Component, class... Params>
    void setComponent(entity::Entity from, Params &&...p);
    void setComponent(entity::Entity entity, const std::type_index &type, const std::any &component);
    void setComponent(const component_info_t &infos);

    template <class Component>
    void destroyComponent(entity::Entity from);
    void destroyComponents(entity::Entity from);

    template <class Component>
    SparseArray<Component> &getComponents(void);
    template <class Component>
    const SparseArray<Component> &getComponents(void) const;

    const component_map_t &getComponentMap(void) const;

    template <class Component>
    std::vector<component_info_t> deltaDiffSparse(const SparseArray<Component> &sparse1,
                                                  const SparseArray<Component> &sparse2) const;

private:
    component_map_t m_componentMap;
};

std::vector<component_info_t> deltaDiff(const Manager::component_map_t &map1, const Manager::component_map_t &map2);
} // namespace ecs::component

#include "Manager.inl"
