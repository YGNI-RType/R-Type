/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Component.hpp
*/

#pragma once

#include "ecs/component/IsComponent.hpp"
#include "ecs/component/Iterator.hpp"

namespace gengine {
template <class Derived>
using Component = ecs::component::IsComponent<Derived>;

template <class... Element>
using Zip = ecs::component::Zipper<Element...>;
} // namespace gengine
