/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** SparseArray.hpp
*/

#pragma once

#include "ecs/entity/Entity.hpp"

#include <limits>
#include <vector>

namespace ecs::component {

template <class Component> class SparseArray {
public:
    SparseArray() = default;
    ~SparseArray() = default;

    void reserve(entity::Entity capacity) { m_sparse.resize(capacity, invalid_index); }

    void insert(entity::Entity entity, const Component &component) {
        if (entity >= m_sparse.size())
            reserve(entity + 1);

        if (m_sparse[entity] == invalid_index) {
            m_sparse[entity] = m_dense.size();
            m_dense.emplace_back(entity, component);
        } else {
            m_dense[m_sparse[entity]].second = component;
        }
    }

    template <typename... Params> void emplace(entity::Entity entity, Params &&...p) {
        if (entity >= m_sparse.size())
            reserve(entity + 1);

        if (m_sparse[entity] == invalid_index) {
            m_sparse[entity] = m_dense.size();
            m_dense.emplace_back(entity, Component(std::forward<Params>(p)...));
        } else {
            m_dense[m_sparse[entity]].second = Component(std::forward<Params>(p)...);
        }
    }

    void erase(entity::Entity entity) {
        if (entity >= m_sparse.size() || m_sparse[entity] == invalid_index)
            return;

        entity::Entity denseIndex = m_sparse[entity];
        entity::Entity lastDenseIndex = m_dense.size() - 1;
        if (denseIndex != lastDenseIndex) {
            std::swap(m_dense[denseIndex], m_dense[lastDenseIndex]);
            m_sparse[m_dense[denseIndex].first] = denseIndex;
        }
        m_dense.pop_back();
        m_sparse[entity] = invalid_index;
    }

    bool contains(entity::Entity entity) const { return entity < m_sparse.size() && m_sparse[entity] != invalid_index; }

    Component &get(entity::Entity entity) { return m_dense[m_sparse[entity]].second; }

    const Component &get(entity::Entity entity) const { return m_dense[m_sparse[entity]].second; }

    std::size_t size() const { return m_dense.size(); }

    auto begin() { return m_dense.begin(); }
    auto end() { return m_dense.end(); }
    auto cbegin() const { return m_dense.cbegin(); }
    auto cend() const { return m_dense.cend(); }

private:
    static constexpr std::size_t invalid_index = std::numeric_limits<std::size_t>::max();

    std::vector<entity::Entity> m_sparse;
    std::vector<std::pair<entity::Entity, Component>> m_dense;
};

} // namespace ecs::component
