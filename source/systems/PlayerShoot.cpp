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
}

void PlayerShoot::shoot(gengine::interface::network::event::RemoteEvent<event::Shoot> &e) {
    if (e->state == event::Shoot::CHARGING && m_lastState == event::Shoot::REST) {
        m_lastCharge = std::chrono::system_clock::now();
        m_lastState = event::Shoot::CHARGING;
        shootBullet();
        return;
    }
    if (e->state == event::Shoot::REST && m_lastState == event::Shoot::CHARGING) {
        m_lastState = event::Shoot::REST;
        if (getChargeDuration() > 500)
            shootBeam();
        return;
    }
}

void PlayerShoot::shootBullet(void) {
    auto &players = getComponents<component::PlayerControl>();
    auto &transforms = getComponents<gengine::component::Transform2D>();

    for (auto [entity, player, transform] : gengine::Zip(players, transforms)) {
        spawnEntity(component::Bullet(),
                    gengine::component::Transform2D({transform.pos.x + 93, transform.pos.y + 22}, {2, 2}, 0),
                    gengine::component::Velocity2D(BULLET_SPEED, 0),
                    gengine::component::driver::output::Sprite("r-typesheet1.gif", Rectangle{248, 85, 17, 12}, WHITE),
                    gengine::component::driver::output::Drawable(1), gengine::component::HitBoxSquare2D(17, 12));
    }
}

void PlayerShoot::shootBeam(void) {
    auto &players = getComponents<component::PlayerControl>();
    auto &transforms = getComponents<gengine::component::Transform2D>();

    for (auto [entity, player, transform] : gengine::Zip(players, transforms)) {
        spawnEntity(component::Bullet(true),
                    gengine::component::Transform2D({transform.pos.x + 93, transform.pos.y + 22},
                                                    {static_cast<float>(2 + getChargeDuration() / 1000.0),
                                                     static_cast<float>(2 + getChargeDuration() / 1000.0)},
                                                    0),
                    gengine::component::Velocity2D(BULLET_SPEED * 2, 0),
                    gengine::component::driver::output::Sprite("r-typesheet1.gif", Rectangle{248, 85, 17, 12}, RED),
                    gengine::component::driver::output::Drawable(1), gengine::component::HitBoxSquare2D(17, 12));
    }
}

long PlayerShoot::getChargeDuration(void) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - m_lastCharge)
        .count();
}

} // namespace rtype::system
