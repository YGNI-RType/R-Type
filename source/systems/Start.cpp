/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Start.hpp
*/

#include "Constants.hpp"

#include "systems/Start.hpp"

#include "GEngine/libdev/Components.hpp"

#include "components/Background.hpp"
#include "components/Boss.hpp"
#include "components/Invincible.hpp"
#include "components/Life.hpp"
#include "components/Monster.hpp"
#include "components/Player.hpp"
#include "components/Score.hpp"
#include "components/ScoreText.hpp"

#include <random>

namespace rtype {
void system::Start::init(void) {
    subscribeToEvent<gengine::system::event::StartEngine>(&Start::onStartEngine);
    subscribeToEvent<gengine::interface::event::NewRemoteLocal>(&Start::onNewRemoteLocal);
    subscribeToEvent<gengine::interface::event::DeleteRemoteLocal>(&Start::onDeleteRemoteLocal);
}

void system::Start::onStartEngine(gengine::system::event::StartEngine &e) {
    spawnEntity(component::ScoreText(), geg::component::Transform2D({1280 - 315, 726}, {2, 2}),
                geg::component::io::Drawable(2), geg::component::io::Text("arcade.ttf", "Score: 0", WHITE),
                geg::component::network::NetSend());

    for (std::size_t i = 0; i < 2; i++) {
        spawnEntity(component::Life(), geg::component::Transform2D({99 - i * 66.f, 726}, {1, 1}, 0),
                    geg::component::io::Drawable(1),
                    geg::component::io::Sprite("spaceships.gif", Rectangle{66, 0, 33, 17}, WHITE),
                    geg::component::network::NetSend());
    }

    float scale = 7;
    float width = 45;
    float height = 23;
    spawnEntity(component::Monster(1000), geg::component::Transform2D({1400, 0}, {scale, scale}, 0),
                geg::component::Velocity2D(-1, 0), geg::component::io::Drawable(3),
                geg::component::io::Sprite("background.png", Rectangle{465, 225, width, height}, WHITE),
                geg::component::HitBoxSquare2D(width, height - 10), geg::component::network::NetSend());

    scale = 7;
    width = 279;
    height = 8;
    spawnEntity(component::Monster(1000), geg::component::HitBoxSquare2D(width, height),
                geg::component::Transform2D({1000, WINDOW_HEIGHT - height * scale}, {scale, scale}, 0),
                geg::component::Velocity2D(-1, 0), geg::component::io::Drawable(3),
                geg::component::io::Sprite("background.png", Rectangle{417, 284, width, height}, WHITE),
                geg::component::network::NetSend());
}

void system::Start::onNewRemoteLocal(gengine::interface::event::NewRemoteLocal &e) {
    spawnEntity(
        component::Player(), component::Invincible(3), geg::component::Transform2D({0, float(rand() % 500)}, {3, 3}, 0),
        geg::component::Velocity2D(0, 0), geg::component::io::Drawable(1),
        geg::component::io::Sprite("spaceships.gif", Rectangle{66, 17.f * (m_nbPlayer % 5), 33, 17}, WHITE),
        geg::component::HitBoxSquare2D(33, 17), gengine::interface::component::RemoteLocal(e.uuid),
        geg::component::io::Animation("spaceships.json/fly", 0.1f, geg::component::io::AnimationTrack::Idling, 2),
        component::Score(0), geg::component::network::NetSend());
    m_nbPlayer++;
}

void system::Start::onDeleteRemoteLocal(gengine::interface::event::DeleteRemoteLocal &e) {
    auto &remotes = getComponents<gengine::interface::component::RemoteLocal>();
    for (auto &[entity, remote] : remotes) {
        if (remote.getUUIDBytes() == e.uuid) {
            killEntity(entity);
            m_nbPlayer--;
            return;
        }
    }
}
} // namespace rtype
