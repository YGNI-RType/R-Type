/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** ComponentSparseArray.hpp
*/

#pragma once

#include "Entity.hpp"

#include <vector>
#include <limits>

namespace ecs {

    template <class Component>
    class ComponentSparseArray {
    public:
        ComponentSparseArray() = default;
        ~ComponentSparseArray() = default;

        void reserve(Entity capacity)
        {
            m_sparse.resize(capacity, invalid_index);
        }

        void insert(Entity entity, const Component &component)
        {
            if (entity >= m_sparse.size())
                reserve(entity + 1);

            if (m_sparse[entity] == invalid_index) {
                m_sparse[entity] = m_dense.size();
                m_dense.emplace_back(entity, component);
            } else {
                m_dense[m_sparse[entity]].second = component;
            }
        }

        template <typename ...Params>
        void emplace(Entity entity, Params &&...p)
        {
            if (entity >= m_sparse.size())
                reserve(entity + 1);

            if (m_sparse[entity] == invalid_index) {
                m_sparse[entity] = m_dense.size();
                m_dense.emplace_back(entity, Component(std::forward<Params>(p)...));
            } else {
                m_dense[m_sparse[entity]].second = Component(std::forward<Params>(p)...);
            }
        }

        void erase(Entity entity)
        {
            if (entity >= m_sparse.size() || m_sparse[entity] == invalid_index)
                return;

            Entity denseIndex = m_sparse[entity];
            Entity lastDenseIndex = m_dense.size() - 1;
            if (denseIndex != lastDenseIndex) {
                std::swap(m_dense[denseIndex], m_dense[lastDenseIndex]);
                m_sparse[m_dense[denseIndex].first] = denseIndex;
            }
            m_dense.pop_back();
            m_sparse[entity] = invalid_index;
        }

        bool contains(Entity entity) const
        {
            return entity < m_sparse.size() && m_sparse[entity] != invalid_index;
        }

        Component& get(Entity entity)
        {
            return m_dense[m_sparse[entity]].second;
        }

        const Component& get(Entity entity) const
        {
            return m_dense[m_sparse[entity]].second;
        }

        std::size_t size() const {
            return m_dense.size();
        }

        auto begin() { return m_dense.begin(); }
        auto end() { return m_dense.end(); }
        auto cbegin() const { return m_dense.cbegin(); }
        auto cend() const { return m_dense.cend(); }

    private:
        static constexpr std::size_t invalid_index = std::numeric_limits<std::size_t>::max();

        std::vector<Entity> m_sparse;
        std::vector<std::pair<Entity, Component>> m_dense;
    };

}
