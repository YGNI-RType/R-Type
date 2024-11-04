/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** BackgroundAutoMotion.hpp
*/

#pragma once

#include "GEngine/libdev/Components.hpp"
#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/systems/events/GameLoop.hpp"

#include "components/Background.hpp"

namespace rtype::system {
class BackgroundMotion
    : public gengine::System<BackgroundMotion, geg::component::Velocity2D, geg::component::io::Sprite,
                             geg::component::Transform2D, component::Background> {
public:
    void init(void) override;
    void onGameLoop(gengine::system::event::GameLoop &);
    void updateBackgroundSpeed(void);
};
} // namespace rtype::system
