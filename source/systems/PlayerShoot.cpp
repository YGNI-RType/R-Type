/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** PlayerShoot.cpp
*/

#include "systems/PlayerShoot.hpp"

#include "GEngine/libdev/Component.hpp" // gengine::Zip

#include <random>

// Spawn entity
#include "GEngine/libdev/components/driver/output/Animation.hpp"
#include "GEngine/libdev/components/driver/output/Drawable.hpp"
#include "GEngine/libdev/components/driver/output/Shape.hpp"
#include "GEngine/libdev/components/driver/output/Sprite.hpp"

namespace rtype::system {
void PlayerShoot::init(void) {
    subscribeToEvent<gengine::system::event::driver::input::Key_B>(&PlayerShoot::shoot);
}

void PlayerShoot::shoot(gengine::system::event::driver::input::Key_B &e) {
    auto &players = getComponents<component::PlayerControl>();
    auto &transforms = getComponents<gengine::component::Transform2D>();

    for (auto [entity, player, transform] : gengine::Zip(players, transforms)) {
        spawnEntity(gengine::component::Transform2D({transform.pos.x + 60, transform.pos.y + 10}, {2, 2}, 0),
                    gengine::component::Velocity2D(100 + rand() % 1000, rand() % 100 - 50),
                    gengine::component::driver::output::Sprite("r-typesheet3.gif", Rectangle{0, 0, 17, 18}, WHITE),
                    gengine::component::driver::output::Animation(12, rand() % 12, 0.1f),
                    gengine::component::driver::output::Drawable(1));
    }
}
} // namespace rtype::system
