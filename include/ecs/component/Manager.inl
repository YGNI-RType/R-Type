/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Manager.inl
*/

#pragma once

namespace ecs::component {
template <class Component> SparseArray<Component> &Manager::registerComponent() {
    auto res = m_componentArrays.emplace(std::type_index(typeid(Component)),
                                         std::make_pair(SparseArray<Component>(), [this](entity::Entity entity) {
                                             destroyComponent<Component>(entity);
                                         }));
    return std::any_cast<SparseArray<Component> &>(res.first->second.first);
}

template <class Component> void Manager::setComponent(entity::Entity entity, const Component &component) {
    getComponents<Component>().insert(entity, component);
}

template <class Component> void Manager::destroyComponent(entity::Entity entity) {
    getComponents<Component>().erase(entity);
}

template <typename Component, class... Params> void Manager::setComponent(entity::Entity entity, Params &&...p) {
    getComponents<Component>().emplace(entity, Component(std::forward<Params>(p)...));
}

template <class Component> SparseArray<Component> &Manager::getComponents() {
    auto it = m_componentArrays.find(std::type_index(typeid(Component)));
    if (it == m_componentArrays.end())
        throw std::runtime_error("The component " + std::string(typeid(Component).name()) +
                                 " does not exist in the Manager");
    return std::any_cast<SparseArray<Component> &>(it->second.first);
}

template <class Component> const SparseArray<Component> &Manager::getComponents() const {
    auto it = m_componentArrays.find(std::type_index(typeid(Component)));
    if (it == m_componentArrays.end())
        throw std::runtime_error("The component " + std::string(typeid(Component).name()) +
                                 " does not exist in the Manager");
    return std::any_cast<const SparseArray<Component> &>(it->second);
}
} // namespace ecs::component
