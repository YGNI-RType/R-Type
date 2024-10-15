/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** PlayerShoot.cpp
*/

#include "systems/PlayerShoot.hpp"
#include "components/Bullet.hpp"

// Spawn entity
#include "GEngine/libdev/Component.hpp" // gengine::Zip
#include "GEngine/libdev/components/Velocities.hpp"
#include "GEngine/libdev/components/driver/output/Animation.hpp"
#include "GEngine/libdev/components/driver/output/Drawable.hpp"
#include "GEngine/libdev/components/driver/output/Shape.hpp"
#include "GEngine/libdev/components/driver/output/Sprite.hpp"

#define BULLET_SPEED 10

namespace rtype::system {
void PlayerShoot::init(void) {
    subscribeToEvent<gengine::interface::network::event::RemoteEvent<event::Shoot>>(&PlayerShoot::shoot);
    subscribeToEvent<gengine::interface::event::NewRemoteDriver>(&PlayerShoot::newShooter);
    subscribeToEvent<gengine::interface::event::DeleteRemoteDriver>(&PlayerShoot::deleteShooter);
}

void PlayerShoot::newShooter(gengine::interface::event::NewRemoteDriver &e) {
    m_shooterMap.insert({e.remote, shoot_infos_t()});
}
void PlayerShoot::deleteShooter(gengine::interface::event::DeleteRemoteDriver &e) {
    m_shooterMap.erase(e.remote);
}

void PlayerShoot::shoot(gengine::interface::network::event::RemoteEvent<event::Shoot> &e) {
    auto it = m_shooterMap.find(e.remote);

    if (it == m_shooterMap.end())
        return;
    auto &[state, lastCharge] = it->second;
    if (e->state == event::Shoot::REST && state == event::Shoot::CHARGING) {
        state = event::Shoot::REST;
        if (getChargeDuration(lastCharge) > 500)
            shootBeam(e.remote, 2 + getChargeDuration(lastCharge) / 1000.f);
        return;
    }
    if (e->state == event::Shoot::CHARGING && state == event::Shoot::REST) {
        lastCharge = std::chrono::system_clock::now();
        state = event::Shoot::CHARGING;
        shootBullet(e.remote);
    }
}

void PlayerShoot::shootBullet(const gengine::interface::component::RemoteDriver &from) {
    auto &players = getComponents<gengine::interface::component::RemoteDriver>();
    auto &transforms = getComponents<gengine::component::Transform2D>();

    for (auto [entity, player, transform] : gengine::Zip(players, transforms)) {
        if (player != from)
            continue;
        spawnEntity(component::Bullet(),
                    gengine::component::Transform2D({transform.pos.x + 93, transform.pos.y + 22}, {2, 2}, 0),
                    gengine::component::Velocity2D(BULLET_SPEED, 0),
                    gengine::component::driver::output::Sprite("r-typesheet1.gif", Rectangle{248, 85, 17, 12}, WHITE),
                    gengine::component::driver::output::Drawable(1), gengine::component::HitBoxSquare2D(17, 12));
    }
}

void PlayerShoot::shootBeam(const gengine::interface::component::RemoteDriver &from, float bulletScale) {
    auto &players = getComponents<gengine::interface::component::RemoteDriver>();
    auto &transforms = getComponents<gengine::component::Transform2D>();

    for (auto [entity, player, transform] : gengine::Zip(players, transforms)) {
        if (player != from)
            continue;
        spawnEntity(component::Bullet(true),
                    gengine::component::Transform2D({transform.pos.x + 93, transform.pos.y + 22},
                                                    {bulletScale, bulletScale}, 0),
                    gengine::component::Velocity2D(BULLET_SPEED * 2, 0),
                    gengine::component::driver::output::Sprite("r-typesheet1.gif", Rectangle{248, 85, 17, 12}, RED),
                    gengine::component::driver::output::Drawable(1), gengine::component::HitBoxSquare2D(17, 12));
    }
}

long PlayerShoot::getChargeDuration(const std::chrono::time_point<std::chrono::system_clock> &lastCharge) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastCharge).count();
}

} // namespace rtype::system
