/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** component::Manager.hpp
*/

#pragma once

#include <any>
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
    Manager();

    template <class Component> SparseArray<Component> &registerComponent();

    template <class Component> void setComponent(entity::Entity from, const Component &component);
    template <typename Component, class... Params> void setComponent(entity::Entity from, Params &&...p);

    template <class Component> void destroyComponent(entity::Entity from);
    void destroyComponents(entity::Entity from);

    template <class Component> SparseArray<Component> &getComponents();
    template <class Component> const SparseArray<Component> &getComponents() const;

private:
    typedef std::function<void(entity::Entity)> destroyer_t;
    std::unordered_map<std::type_index, std::pair<std::any, destroyer_t>> m_componentArrays;
};

} // namespace ecs::component

#include "Manager.inl"
