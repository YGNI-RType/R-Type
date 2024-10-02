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
using Component = ecs::component::IsComponent;

template <class... Element> using Zip = ecs::component::Zipper<Element...>;
} // namespace gengine
