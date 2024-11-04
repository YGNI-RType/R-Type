/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Start.hpp
*/

#include "Constants.hpp"

#include "systems/Lobby.hpp"

#include "components/Background.hpp"
#include "components/Boss.hpp"
#include "components/Invincible.hpp"
#include "components/Life.hpp"
#include "components/Monster.hpp"
#include "components/Score.hpp"
#include "components/ScoreText.hpp"

#include "GEngine/libdev/systems/events/gui/Scenes.hpp"

#include <random>

namespace rtype::system {
void Lobby::init(void) {
    subscribeToEvent<gengine::system::event::StartEngine>(&Lobby::initGame);
    subscribeToEvent<gengine::system::event::GameLoop>(&Lobby::checkPlayersReady);
    subscribeToEvent<event::StartGame>(&Lobby::onStartGame);
    subscribeToEvent<event::GameOver>(&Lobby::onGameOver);
    subscribeToEvent<event::NextStage>(&Lobby::onNextStage);
    subscribeToEvent<gengine::interface::event::NewRemoteLocal>(&Lobby::onNewRemoteLocal);
    subscribeToEvent<gengine::interface::event::DeleteRemoteLocal>(&Lobby::onDeleteRemoteLocal);
    subscribeToEvent<gengine::interface::event::SharedEvent<event::IAmReady>>(&Lobby::setPlayersReady);
}

void Lobby::initGame(gengine::system::event::StartEngine &e) {
    spawnEntity(component::GameState(component::GameState::LOBBY), geg::component::network::NetSend());
    spawnEntity(component::ScoreText(), geg::component::Transform2D({1280 - 315, 726}, {2, 2}),
                geg::component::io::Drawable(2), geg::component::io::Text("arcade.ttf", "Score: 0", WHITE),
                geg::component::network::NetSend());
}

void Lobby::setPlayersReady(gengine::interface::event::SharedEvent<event::IAmReady> &e) {
    if (m_started)
        return;

    auto &[uuid, infos] = *m_playersInLobby.find(e.remoteUUID);
    infos.second = e->ready;
}

void Lobby::checkPlayersReady(gengine::system::event::GameLoop &e) {
    if (m_started || !m_playersInLobby.size())
        return;

    for (auto &[uuid, infos] : m_playersInLobby)
        if (!infos.second)
            return;
    publishEvent(event::StartGame());
    auto &states = getComponents<component::GameState>();
    auto &netsends = getComponents<geg::component::network::NetSend>();
    for (auto [e, state, netsend] : gengine::Zip(states, netsends)) {
        state = component::GameState::GAME;
        netsend.update();
    }
    m_started = true;
}

void Lobby::onGameOver(event::GameOver &) {
    m_started = false;
    for (auto &[uuid, infos] : m_playersInLobby)
        infos.second = false;
    auto &states = getComponents<component::GameState>();
    auto &netsends = getComponents<geg::component::network::NetSend>();
    for (auto [e, state, netsend] : gengine::Zip(states, netsends)) {
        state = component::GameState::GAMEOVER;
        netsend.update();
    }
}

void Lobby::onNewRemoteLocal(gengine::interface::event::NewRemoteLocal &e) {
    size_t playerNb = m_playersInLobby.size();
    m_playersInLobby.emplace(e.uuid, std::make_pair(playerNb, false));
    if (m_started)
        return;
    spawnPlayer(e.uuid, playerNb, 3);
}

void Lobby::spawnPlayer(const uuids::uuid &remoteUUID, size_t playerNb, size_t lifes) {
    spawnEntity(
        component::Player(5.f, lifes), geg::component::Transform2D({0, WINDOW_HEIGHT / 2 - (17 * 3) / 2.0f}, {3, 3}, 0),
        geg::component::Velocity2D(0, 0), geg::component::io::Drawable(1),
        geg::component::io::Sprite("spaceships.gif", Rectangle{66, 17.f * (playerNb % 4), 33, 17}),
        geg::component::HitBoxSquare2D(33, 17), gengine::interface::component::RemoteLocal(remoteUUID),
        geg::component::io::Animation("spaceships.json/fly", 0.1f, geg::component::io::AnimationTrack::Idling, 2),
        component::Score(0), geg::component::network::NetSend());
}

void Lobby::onDeleteRemoteLocal(gengine::interface::event::DeleteRemoteLocal &e) {
    auto &remotes = getComponents<gengine::interface::component::RemoteLocal>();

    for (auto &[entity, remote] : remotes) {
        if (remote.getUUIDBytes() == e.uuid) {
            killEntity(entity);
            m_playersInLobby.erase(e.uuid);
            return;
        }
    }
}

void Lobby::onStartGame(event::StartGame &) {
    respawnPlayers(3);
}

void Lobby::onNextStage(event::NextStage &) {
    respawnPlayers(1);
}

void Lobby::respawnPlayers(size_t playerLife) {
    auto &remotes = getComponents<gengine::interface::component::RemoteLocal>();

    for (auto &[uuid, infos] : m_playersInLobby) {
        auto &[playerNb, ready] = infos;
        bool found = false;
        for (auto &[entity, remote] : remotes) {
            if (uuid == remote.getUUIDBytes()) {
                found = true;
                break;
            }
        }
        if (!found)
            spawnPlayer(uuid, playerNb, playerLife);
    }
}
} // namespace rtype::system
