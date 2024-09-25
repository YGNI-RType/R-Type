/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** System.hpp
*/

#pragma once

#include "ecs/system/Base.hpp"
#include "ecs/system/event/Base.hpp"

namespace gengine {
template <typename T, class... DependTypes> using System = ecs::system::Base<T, DependTypes...>;
using Event = ecs::system::event::Base;
} // namespace gengine
