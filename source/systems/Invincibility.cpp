/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Invincibility.cpp
*/

#include "systems/Invincibility.hpp"

#include "GEngine/libdev/Component.hpp" // gengine::Zip

namespace rtype::system {
void Invincibility::init(void) {
    subscribeToEvent<gengine::system::event::GameLoop>(&Invincibility::onGameLoop);
    subscribeToEvent<gengine::interface::event::SharedEvent<event::BecomeInvincible>>(&Invincibility::becomeInvincible);
}

void Invincibility::onGameLoop(gengine::system::event::GameLoop &e) {
    auto &invincibles = getComponents<component::Invincible>();
    auto &sprites = getComponents<geg::component::io::Sprite>();

    for (auto &[entity, invincible] : invincibles) {
        // std::cout << "Invincibility: " << invincible.duration << std::endl;
        if (invincible.duration > 0) {
            invincible.duration -= e.deltaTime / 1000;
        } else {
            unsetComponent<component::Invincible>(entity);
            if (sprites.contains(entity))
                sprites.get(entity).tint.a = 255;
        }
    }
}

void Invincibility::becomeInvincible(gengine::interface::event::SharedEvent<event::BecomeInvincible> &e) {
    // std::cout << "Event BecomeInvincible" << std::endl;
    auto &players = getComponents<gengine::interface::component::RemoteLocal>();
    auto &sprites = getComponents<geg::component::io::Sprite>();
    if (e->state) {
        for (auto &[entity, player] : players) {
            setComponent(entity, component::Invincible(999999));
            if (sprites.contains(entity))
                sprites.get(entity).tint.a = 128;
        }
    }
}
} // namespace rtype::system
