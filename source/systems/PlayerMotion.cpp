/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** PlayerMotion.cpp
*/

#include "systems/PlayerMotion.hpp"

#include "GEngine/libdev/components/Velocities.hpp"
#include "GEngine/libdev/components/driver/output/Animation.hpp"

#include "GEngine/libdev/Component.hpp" // gengine::Zip

#include <random>

namespace rtype::system {
void PlayerMotion::init(void) {
    // subscribeToEvent<gengine::interface::network::event::RemoteEvent<gengine::system::event::driver::input::Key_P>>(
    //     &PlayerMotion::increaseSpeed);
    // subscribeToEvent<gengine::interface::network::event::RemoteEvent<gengine::system::event::driver::input::Key_O>>(
    //     &PlayerMotion::decreaseSpeed);
    subscribeToEvent<gengine::interface::network::event::RemoteEvent<event::Movement>>(&PlayerMotion::movePlayer);
}
void PlayerMotion::movePlayer(gengine::interface::network::event::RemoteEvent<event::Movement> &e) {
    auto &velocities = getComponents<gengine::component::Velocity2D>();
    auto &players = getComponents<component::Player>();
    auto &playerControls = getComponents<component::PlayerControl>();

    for (auto [entity, player, velocity, playerControl] : gengine::Zip(players, velocities, playerControls)) {
        if (e.remote != e.remote) // check if its the same remote (zip)
            continue;
        switch (e->state) {
        case event::Movement::LEFT:
            velocity = {-player.speed, 0};
            break;
        case event::Movement::RIGHT:
            velocity = {player.speed, 0};
            break;
        case event::Movement::UP:
            velocity = {0, -player.speed};
            break;
        case event::Movement::DOWN:
            velocity = {0, player.speed};
            break;
        case event::Movement::UP_RIGHT:
            velocity = {player.speed, -player.speed};
            break;
        case event::Movement::UP_LEFT:
            velocity = {-player.speed, -player.speed};
            break;
        case event::Movement::DOWN_RIGHT:
            velocity = {player.speed, player.speed};
            break;
        case event::Movement::DOWN_LEFT:
            velocity = {-player.speed, player.speed};
            break;
        case event::Movement::STANDING:
            velocity = {0, 0};
            break;
        }
    }
}

// void PlayerMotion::increaseSpeed(
//     gengine::interface::network::event::RemoteEvent<gengine::system::event::driver::input::Key_P> &e) {
//     auto &playerControls = getComponents<component::PlayerControl>();
//     auto &players = getComponents<component::Player>();

//     for (auto [entity, player, playerControl] : gengine::Zip(players, playerControls))
//         player.speed += 10;
// }

// void PlayerMotion::decreaseSpeed(
//     gengine::interface::network::event::RemoteEvent<gengine::system::event::driver::input::Key_O> &e) {
//     auto &players = getComponents<component::Player>();
//     auto &playerControls = getComponents<component::PlayerControl>();

//     for (auto [entity, player, playerControl] : gengine::Zip(players, playerControls))
//         player.speed = std::max(player.speed - 10, 20.f);
// }
} // namespace rtype::system
