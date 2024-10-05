/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Component.hpp
*/

#pragma once

#include "ecs/component/IsComponent.hpp"
#include "ecs/component/Iterator.hpp"

#include <utility>

namespace gengine {
template <class Derived>
using Component = ecs::component::IsComponent<Derived>;

template <typename SparseArrayType>
struct SparseArrayTraits;

template <typename Element>
struct SparseArrayTraits<ecs::component::SparseArray<Element>> {
    using type = Element;
};

template <class... SparseArrays>
ecs::component::Zipper<typename SparseArrayTraits<SparseArrays>::type...> Zip(SparseArrays &...s) {
    return ecs::component::Zipper<typename SparseArrayTraits<SparseArrays>::type...>(s...);
}
} // namespace gengine
