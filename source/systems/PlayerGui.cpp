/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** PlayerGui.cpp
*/

#include "systems/PlayerGui.hpp"

#include "Constants.hpp"
#include "GEngine/libdev/Components.hpp"

namespace rtype::system {
void PlayerGui::init(void) {
    subscribeToEvent<gengine::interface::network::event::ConnectToServer>(&PlayerGui::onConnection);
    subscribeToEvent<event::Shoot>(&PlayerGui::beamCharge);
    subscribeToEvent<gengine::system::event::GameLoop>(&PlayerGui::displayLife);
    subscribeToEvent<gengine::system::event::GameLoop>(&PlayerGui::displayInvincibility);
    subscribeToEvent<gengine::interface::event::ItsMe>(&PlayerGui::setMe);
}

void PlayerGui::onConnection(gengine::interface::network::event::ConnectToServer &) {
    spawnEntity(geg::component::io::Rectangle(400, HUD_HEIGHT, GRAY), geg::component::io::Drawable(8),
                geg::component::Transform2D({WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT}, {1, 1}));
    spawnEntity(geg::component::io::Rectangle(390, HUD_HEIGHT - 10, DARKBLUE), geg::component::io::Drawable(9),
                geg::component::Transform2D({WINDOW_WIDTH / 2 - 195, WINDOW_HEIGHT + 5}, {1, 1}));
    m_bar = spawnEntity(geg::component::io::Rectangle(0, HUD_HEIGHT - 10, BLUE), geg::component::io::Drawable(10),
                        geg::component::Transform2D({WINDOW_WIDTH / 2 - 195, WINDOW_HEIGHT + 5}, {1, 1}));
    spawnEntity(geg::component::io::Text("arcade.ttf", "BEAM", WHITE), geg::component::io::Drawable(11),
                geg::component::Transform2D({WINDOW_WIDTH / 2 - 35, WINDOW_HEIGHT + 5}, {2, 2}));
}

void PlayerGui::displayLife(gengine::system::event::GameLoop &e) {
    if (m_me.is_nil())
        return;

    auto &players = getComponents<component::Player>();
    auto &lifes = getComponents<component::Life>();
    auto &remotes = getComponents<gengine::interface::component::RemoteLocal>();

    m_dead = true;
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
        m_dead = false;
    }
}

void PlayerGui::beamCharge(event::Shoot &e) {
    auto &rects = getComponents<geg::component::io::Rectangle>();
    if (!rects.contains(m_bar))
        return;
    auto &rect = rects.get(m_bar);
    if (m_dead) {
        rect.width = 0;
        return;
    }
    switch (e.state) {
    case geg::event::io::InputState::PRESSED:
        if (rect.width < 390)
            rect.width += 5;
        break;
    case geg::event::io::InputState::RELEASE:
        rect.width = 0;
        break;
    default:
        break;
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
