/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Manager.hpp
*/

#pragma once

#include <any>
#include <functional>
#include <typeindex>
#include <unordered_map>

#include "ecs/component/SparseArray.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

namespace ecs::component {
class Manager {
public:
    Manager(){};

    template <class Component> SparseArray<Component> &registerComponent() {
        auto res = m_componentArrays.emplace(std::type_index(typeid(Component)),
                                             std::make_pair(SparseArray<Component>(), [this](entity::Entity entity) {
                                                 destroyComponent<Component>(entity);
                                             }));
        return std::any_cast<SparseArray<Component> &>(res.first->second.first);
    }

    template <class Component> void setComponent(entity::Entity entity, const Component &component) {
        getComponents<Component>().insert(entity, component);
    }

    void destroyComponents(entity::Entity entity) {
        for (auto &[_, pair] : m_componentArrays)
            pair.second(entity);
    }

    template <class Component> void destroyComponent(entity::Entity entity) {
        getComponents<Component>().erase(entity);
    }

    template <typename Component, class... Params> void setComponent(entity::Entity entity, Params &&...p) {
        getComponents<Component>().emplace(entity, Component(std::forward<Params>(p)...));
    }

    template <class Component> SparseArray<Component> &getComponents() {
        auto it = m_componentArrays.find(std::type_index(typeid(Component)));
        if (it == m_componentArrays.end())
            throw std::runtime_error("The component " + std::string(typeid(Component).name()) +
                                     " does not exist in the Manager");
        return std::any_cast<SparseArray<Component> &>(it->second.first);
    }

    template <class Component> const SparseArray<Component> &getComponents() const {
        auto it = m_componentArrays.find(std::type_index(typeid(Component)));
        if (it == m_componentArrays.end())
            throw std::runtime_error("The component " + std::string(typeid(Component).name()) +
                                     " does not exist in the Manager");
        return std::any_cast<const SparseArray<Component> &>(it->second);
    }

private:
    typedef std::function<void(entity::Entity)> destroyer_t;
    std::unordered_map<std::type_index, std::pair<std::any, destroyer_t>> m_componentArrays;
};

} // namespace ecs::component
