/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** component::Manager.cpp
*/

#include "ecs/component/Manager.hpp"

using namespace ecs;

component::Manager::Manager() {
}

void component::Manager::setComponent(entity::Entity entity, const std::type_index &type, const std::any &component) {
    auto it = m_tools.find(type);
    if (it == m_tools.end())
        THROW_ERROR("The component " + std::string(type.name()) + " does not exist in the Manager");
    auto &[_, setter] = it->second;
    setter(entity, component);
}

void component::Manager::destroyComponents(entity::Entity entity) {
    for (auto it : m_tools) {
        auto &[destroyer, _] = it.second;
        destroyer(entity);
    }
}
