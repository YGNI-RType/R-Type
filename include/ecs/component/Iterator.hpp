/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Iterator.hpp
*/

#pragma once

#include <tuple>
#include <variant>

#include "ecs/component/SparseArray.hpp"

namespace ecs::component {
template <class... Element> class Zipper {
private:
    class Iterator {
    public:
        template <class Component> using iterator_t = SparseArray<Component>::dense_iterator;
        Iterator(std::tuple<SparseArray<Element> &...> &tuple);
        Iterator(std::tuple<SparseArray<Element> &...> &tuple, bool end);

        Iterator operator++(void);

        bool operator==(const Iterator &other) const;

        bool operator!=(const Iterator &other) const;

        std::tuple<const entity::Entity, Element &...> operator*(void);

    private:
        std::tuple<SparseArray<Element> &...> &m_arrays;

        std::variant<iterator_t<Element>...> m_smallestIt;
        std::size_t m_idx = 0;
        std::size_t m_max;

        template <std::size_t... I> bool m_hasAllElement(entity::Entity entity, std::index_sequence<I...>) const;

        template <std::size_t... I>
        std::tuple<const entity::Entity, Element &...> m_getAllElement(entity::Entity entity,
                                                                       std::index_sequence<I...>) const;
    };

public:
    Zipper(SparseArray<Element> &...elem);

    Iterator begin(void);
    Iterator end(void);

private:
    std::tuple<SparseArray<Element> &...> m_elements;
};
} // namespace ecs::component

#include "Iterator.inl"
