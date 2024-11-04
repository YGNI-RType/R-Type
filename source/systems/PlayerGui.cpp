/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** PlayerGui.cpp
*/

#include "systems/PlayerGui.hpp"

#include "GEngine/libdev/Components.hpp"

namespace rtype::system {
void PlayerGui::init(void) {
    subscribeToEvent<gengine::system::event::GameLoop>(&PlayerGui::displayLife);
    subscribeToEvent<gengine::system::event::GameLoop>(&PlayerGui::displayInvincibility);
    subscribeToEvent<gengine::interface::event::ItsMe>(&PlayerGui::setMe);
}

void PlayerGui::displayLife(gengine::system::event::GameLoop &e) {
    if (m_me.is_nil())
        return;

    auto &players = getComponents<component::Player>();
    auto &lifes = getComponents<component::Life>();
    auto &remotes = getComponents<gengine::interface::component::RemoteLocal>();

    for (auto [entity, player, remote] : gengine::Zip(players, remotes)) {
        if (m_me != remote.getUUIDBytes())
            continue;
        for (; m_life < player.lifes; m_life++) {
            spawnEntity(component::Life(), geg::component::Transform2D({150 - m_life * 66.f, 726}, {1, 1}, 0),
                        geg::component::io::Drawable(1),
                        geg::component::io::Sprite("spaceships.gif", Rectangle{66, 0, 33, 17}, WHITE));
        }

        for (auto &[entity, life] : lifes) {
            if (m_life <= player.lifes)
                continue;

            killEntity(entity);
            if (m_life > 1)
                m_life--;
        }
    }
}

void PlayerGui::displayInvincibility(gengine::system::event::GameLoop &e) {
    if (m_me.is_nil())
        return;

    auto &invincibles = getComponents<component::Invincible>();
    auto &sprites = getComponents<geg::component::io::Sprite>();
    auto &remotes = getComponents<gengine::interface::component::RemoteLocal>();

    for (auto [entity, sprite, remote] : gengine::Zip(sprites, remotes))
        if (invincibles.contains(entity))
            sprite.tint.a = 128;
        else
            sprite.tint.a = 255;
}
void PlayerGui::setMe(gengine::interface::event::ItsMe &e) {
    m_me = e.myUUID;
}
} // namespace rtype::system
