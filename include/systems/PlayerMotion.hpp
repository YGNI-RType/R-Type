/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** PlayerMotion.hpp
*/

#pragma once

#include "components/Player.hpp"
#include "components/PlayerControl.hpp"

#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/components/Velocities.hpp"
#include "GEngine/libdev/components/driver/output/Animation.hpp"
#include "GEngine/libdev/systems/events/MainLoop.hpp"
#include "GEngine/libdev/systems/events/driver/input/Keyboard.hpp"

namespace rtype::system {
class PlayerMotion : public gengine::System<PlayerMotion, gengine::component::Velocity2D, component::PlayerControl,
                                            component::Player> {
public:
    void init(void) override;
    void movePlayerLeft(gengine::system::event::driver::input::Key_Left &);
    void movePlayerRight(gengine::system::event::driver::input::Key_Right &);
    void movePlayerUp(gengine::system::event::driver::input::Key_Up &);
    void movePlayerDown(gengine::system::event::driver::input::Key_Down &);
    void increaseSpeed(gengine::system::event::driver::input::Key_P &);
    void decreaseSpeed(gengine::system::event::driver::input::Key_O &);
};
} // namespace rtype::system
