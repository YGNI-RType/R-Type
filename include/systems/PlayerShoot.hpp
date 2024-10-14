/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** PlayerShoot.hpp
*/

#pragma once

#include "GEngine/libdev/components/HitBoxs.hpp"
#include "GEngine/libdev/components/Transforms.hpp"
#include "components/PlayerControl.hpp"

#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/systems/events/GameLoop.hpp"

#include "GEngine/interface/events/RemoteEvent.hpp"
#include "events/Shoot.hpp"

#include "GEngine/interface/events/RemoteEvent.hpp"

namespace rtype::system {
class PlayerShoot : public gengine::System<PlayerShoot, component::PlayerControl, gengine::component::Transform2D,
                                           gengine::component::HitBoxSquare2D> {
public:
    void init(void) override;
    void shoot(gengine::interface::network::event::RemoteEvent<event::Shoot> &);
};
} // namespace rtype::system
