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
    subscribeToEvent<gengine::interface::network::event::RemoteEvent<gengine::system::event::driver::input::Key_Left>>(
        &PlayerMotion::movePlayerLeft);
    subscribeToEvent<gengine::interface::network::event::RemoteEvent<gengine::system::event::driver::input::Key_Right>>(
        &PlayerMotion::movePlayerRight);
    subscribeToEvent<gengine::interface::network::event::RemoteEvent<gengine::system::event::driver::input::Key_Up>>(
        &PlayerMotion::movePlayerUp);
    subscribeToEvent<gengine::interface::network::event::RemoteEvent<gengine::system::event::driver::input::Key_Down>>(
        &PlayerMotion::movePlayerDown);
    subscribeToEvent<gengine::interface::network::event::RemoteEvent<gengine::system::event::driver::input::Key_P>>(
        &PlayerMotion::increaseSpeed);
    subscribeToEvent<gengine::interface::network::event::RemoteEvent<gengine::system::event::driver::input::Key_O>>(
        &PlayerMotion::decreaseSpeed);
}

void PlayerMotion::movePlayerLeft(
    gengine::interface::network::event::RemoteEvent<gengine::system::event::driver::input::Key_Left> &e) {
    auto &velocities = getComponents<gengine::component::Velocity2D>();
    auto &players = getComponents<component::Player>();
    auto &playerControls = getComponents<component::PlayerControl>();

    for (auto [entity, player, velocity, playerControl] : gengine::Zip(players, velocities, playerControls))
        if (e->state == gengine::system::event::driver::input::RELEASE)
            velocity.x += player.speed;
        else if (e->state == gengine::system::event::driver::input::PRESSED)
            velocity.x -= player.speed;
}

void PlayerMotion::movePlayerRight(
    gengine::interface::network::event::RemoteEvent<gengine::system::event::driver::input::Key_Right> &e) {
    auto &velocities = getComponents<gengine::component::Velocity2D>();
    auto &players = getComponents<component::Player>();
    auto &playerControls = getComponents<component::PlayerControl>();

    for (auto [entity, player, velocity, playerControl] : gengine::Zip(players, velocities, playerControls))
        if (e->state == gengine::system::event::driver::input::RELEASE)
            velocity.x -= player.speed;
        else if (e->state == gengine::system::event::driver::input::PRESSED)
            velocity.x += player.speed;
}

void PlayerMotion::movePlayerUp(
    gengine::interface::network::event::RemoteEvent<gengine::system::event::driver::input::Key_Up> &e) {
    auto &velocities = getComponents<gengine::component::Velocity2D>();
    auto &players = getComponents<component::Player>();
    auto &playerControls = getComponents<component::PlayerControl>();

    for (auto [entity, player, velocity, playerControls] : gengine::Zip(players, velocities, playerControls))
        if (e->state == gengine::system::event::driver::input::RELEASE)
            velocity.y += player.speed;
        else if (e->state == gengine::system::event::driver::input::PRESSED)
            velocity.y -= player.speed;
}

void PlayerMotion::movePlayerDown(
    gengine::interface::network::event::RemoteEvent<gengine::system::event::driver::input::Key_Down> &e) {
    auto &velocities = getComponents<gengine::component::Velocity2D>();
    auto &players = getComponents<component::Player>();
    auto &playerControls = getComponents<component::PlayerControl>();

    for (auto [entity, player, velocity, playerControl] : gengine::Zip(players, velocities, playerControls))
        if (e->state == gengine::system::event::driver::input::RELEASE)
            velocity.y -= player.speed;
        else if (e->state == gengine::system::event::driver::input::PRESSED)
            velocity.y += player.speed;
}

void PlayerMotion::increaseSpeed(
    gengine::interface::network::event::RemoteEvent<gengine::system::event::driver::input::Key_P> &e) {
    auto &playerControls = getComponents<component::PlayerControl>();
    auto &players = getComponents<component::Player>();

    for (auto [entity, player, playerControl] : gengine::Zip(players, playerControls))
        player.speed += 10;
}

void PlayerMotion::decreaseSpeed(
    gengine::interface::network::event::RemoteEvent<gengine::system::event::driver::input::Key_O> &e) {
    auto &players = getComponents<component::Player>();
    auto &playerControls = getComponents<component::PlayerControl>();

    for (auto [entity, player, playerControl] : gengine::Zip(players, playerControls))
        player.speed = std::max(player.speed - 10, 20.f);
}
} // namespace rtype::system
