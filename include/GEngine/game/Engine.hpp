/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Engine.hpp
*/

#pragma once

#include "GEngine/BaseEngine.hpp"
#include "ecs/ECS.hpp"
#include <memory>

namespace gengine::game {
    class Engine : public ecs::ECS, public BaseEngine {
    public:
        Engine(std::unique_ptr<gengine::interface::Base> interface);
    };
}
