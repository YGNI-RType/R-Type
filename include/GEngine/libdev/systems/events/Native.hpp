/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Native.hpp
*/

#pragma once

#include "GEngine/libdev/System.hpp"

namespace gengine::system::event {
class MainLoop : public Event {};

class StartEngine : public Event {};
} // namespace gengine::system::event
