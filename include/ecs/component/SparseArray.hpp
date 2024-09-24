/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** SparseArray.hpp
*/

#pragma once

#include <limits>
#include <vector>

#include "ecs/entity/Entity.hpp"

namespace ecs::component {

template <class Component> class SparseArray {
public:
    SparseArray() = default;
    ~SparseArray() = default;

    void reserve(entity::Entity capacity);

    void insert(entity::Entity entity, const Component &component);

    template <typename... Params> void emplace(entity::Entity entity, Params &&...p);

    void erase(entity::Entity entity);

    bool contains(entity::Entity entity) const;

    Component &get(entity::Entity entity);

    const Component &get(entity::Entity entity) const;

    std::size_t size() const;

    using dense_iterator_t = std::vector<std::pair<entity::Entity, Component>>::iterator;
    using dense_const_iterator_t = std::vector<std::pair<entity::Entity, Component>>::const_iterator;
    dense_iterator_t begin();
    dense_iterator_t end();
    dense_const_iterator_t cbegin() const;
    dense_const_iterator_t cend() const;

private:
    static constexpr std::size_t invalid_index = std::numeric_limits<std::size_t>::max();

    std::vector<entity::Entity> m_sparse;
    std::vector<std::pair<entity::Entity, Component>> m_dense;
};
} // namespace ecs::component

#include "SparseArray.inl"
