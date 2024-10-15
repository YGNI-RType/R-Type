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

private:
    void shootBullet(void);
    void shootBeam(void);

    std::chrono::time_point<std::chrono::system_clock> m_lastCharge;
    event::Shoot::State m_lastState;
};
} // namespace rtype::system
