/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** MainLoop.hpp
*/

#pragma once

#include "GEngine/libdev/System.hpp"

namespace gengine::system::event {
class MainLoop : public Event {};

class StopMainLoop : public Event {};
} // namespace gengine::system::event
