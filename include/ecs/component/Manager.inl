/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Manager.inl
*/

#pragma once

namespace ecs::component {
template <class T>
SparseArray<T> &Manager::registerComponent() {
    static_assert(std::is_base_of<ecs::component::IsComponent, T>::value, "T must inherit from component::Base");
    auto res = m_componentArrays.emplace(std::type_index(typeid(T)), SparseArray<T>());
    m_tools.emplace(std::type_index(typeid(T)),
                    std::make_tuple([this](entity::Entity entity) { destroyComponent<T>(entity); },
                                    [this](entity::Entity entity, const std::any &any) {
                                        setComponent<T>(entity, std::any_cast<const T &>(any));
                                    }));
    return std::any_cast<SparseArray<T> &>(res.first->second);
}

template <class T>
void Manager::setComponent(entity::Entity entity, const T &component) {
    getComponents<T>().insert(entity, component);
}

template <typename T, class... Params>
void Manager::setComponent(entity::Entity entity, Params &&...p) {
    getComponents<T>().emplace(entity, T(std::forward<Params>(p)...));
}

template <class T>
void Manager::destroyComponent(entity::Entity entity) {
    getComponents<T>().erase(entity);
}

template <class T>
SparseArray<T> &Manager::getComponents(void) {
    auto it = m_componentArrays.find(std::type_index(typeid(T)));
    if (it == m_componentArrays.end())
        THROW_ERROR("The component " + std::string(READABLE_TYPE_NAME(T)) + " does not exist in the Manager");
    return std::any_cast<SparseArray<T> &>(it->second);
}

template <class T>
const SparseArray<T> &Manager::getComponents(void) const {
    auto it = m_componentArrays.find(std::type_index(typeid(T)));
    if (it == m_componentArrays.end())
        THROW_ERROR("The component " + std::string(READABLE_TYPE_NAME(T)) + " does not exist in the Manager");
    return std::any_cast<const SparseArray<T> &>(it->second);
}
} // namespace ecs::component
