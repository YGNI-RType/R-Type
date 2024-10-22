/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Invincibility.hpp
*/

#pragma once

#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/systems/events/GameLoop.hpp"

#include "GEngine/interface/events/RemoteDriver.hpp"
#include "GEngine/interface/events/RemoteEvent.hpp"

#include "components/Invincible.hpp"

#include "events/BecomeInvincible.hpp"

namespace rtype::system {

class Invincibility
    : public gengine::System<Invincibility, component::Invincible, gengine::interface::component::RemoteDriver> {
public:
    void init(void) override;
    void onGameLoop(gengine::system::event::GameLoop &);
    void becomeInvincible(gengine::interface::network::event::RemoteEvent<event::BecomeInvincible> &e);
};
} // namespace rtype::system
