/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Engine.cpp
*/

#include "GEngine/game/Engine.hpp"

namespace gengine::game {
    Engine::Engine(std::unique_ptr<gengine::interface::Base> interface) : BaseEngine(std::move(interface)) {

    }
}
