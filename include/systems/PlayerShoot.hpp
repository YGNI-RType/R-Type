/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** PlayerShoot.hpp
*/

#pragma once

#include "GEngine/interface/components/RemoteDriver.hpp"
#include "GEngine/libdev/components/HitBoxs.hpp"
#include "GEngine/libdev/components/Transforms.hpp"

#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/systems/events/GameLoop.hpp"

#include "GEngine/interface/events/RemoteDriver.hpp"
#include "GEngine/interface/events/RemoteEvent.hpp"
#include "events/Shoot.hpp"

namespace rtype::system {
class PlayerShoot : public gengine::System<PlayerShoot, gengine::interface::component::RemoteDriver,
                                           gengine::component::Transform2D> {
public:
    void init(void) override;
    void newShooter(gengine::interface::event::NewRemoteDriver &e);
    void deleteShooter(gengine::interface::event::DeleteRemoteDriver &e);
    void shoot(gengine::interface::network::event::RemoteEvent<event::Shoot> &);

private:
    void shootBullet(const gengine::interface::component::RemoteDriver &from);
    void shootBeam(const gengine::interface::component::RemoteDriver &from, float bulletScale);
    long getChargeDuration(const std::chrono::time_point<std::chrono::system_clock> &lastCharge);

    using shoot_infos_t = std::pair<event::Shoot::State, std::chrono::time_point<std::chrono::system_clock>>;
    std::unordered_map<gengine::interface::component::RemoteDriver, shoot_infos_t> m_shooterMap;
};
} // namespace rtype::system
