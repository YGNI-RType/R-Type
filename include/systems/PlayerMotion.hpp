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
#include "GEngine/libdev/components/Transforms.hpp"
#include "GEngine/libdev/components/Velocities.hpp"
#include "GEngine/libdev/components/driver/output/Animation.hpp"
#include "GEngine/libdev/systems/events/GameLoop.hpp"
#include "GEngine/libdev/systems/events/driver/input/Keyboard.hpp"

#include "GEngine/interface/events/RemoteEvent.hpp"
#include "events/Movement.hpp"

namespace rtype::system {
class PlayerMotion : public gengine::System<PlayerMotion, gengine::component::Velocity2D, component::PlayerControl,
                                            gengine::component::Transform2D, component::Player> {
public:
    void init(void) override;
    void onGameLoop(gengine::system::event::GameLoop &);
    void movePlayer(gengine::interface::network::event::RemoteEvent<event::Movement> &e);
};
} // namespace rtype::system
