/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Start.hpp
*/

#pragma once

#include "GEngine/interface/components/RemoteLocal.hpp"
#include "GEngine/interface/events/RemoteLocal.hpp"
#include "GEngine/interface/events/SharedEvent.hpp"
#include "GEngine/libdev/Components.hpp"
#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/systems/events/GameLoop.hpp"
#include "GEngine/libdev/systems/events/Native.hpp"

#include "components/GameState.hpp"
#include "components/Player.hpp"
#include "components/Score.hpp"
#include "components/ScoreText.hpp"
#include "events/GameOver.hpp"
#include "events/NextStage.hpp"
#include "events/StartGame.hpp"

namespace rtype::system {
class Lobby
    : public gengine::System<Lobby, component::Player, component::Score, component::ScoreText, component::GameState,
                             geg::component::Velocity2D, geg::component::Transform2D, geg::component::HitBoxSquare2D,
                             geg::component::io::Sprite, geg::component::io::Animation, geg::component::io::Drawable,
                             geg::component::io::Text, gengine::interface::component::RemoteLocal,
                             geg::component::network::NetSend>,
      public gengine::RemoteSystem {
public:
    void init(void) override;

    void initGame(gengine::system::event::StartEngine &);
    void setPlayersReady(gengine::interface::event::SharedEvent<event::IAmReady> &);
    void checkPlayersReady(gengine::system::event::GameLoop &);
    void onGameOver(event::GameOver &);
    void onNewRemoteLocal(gengine::interface::event::NewRemoteLocal &);
    void onDeleteRemoteLocal(gengine::interface::event::DeleteRemoteLocal &);
    void respawnPlayers(size_t playerLife);
    void onStartGame(event::StartGame &);
    void onNextStage(event::NextStage &);

private:
    std::unordered_map<uuids::uuid, std::pair<size_t, bool>> m_playersInLobby;
    void spawnPlayer(const uuids::uuid &remoteUUID, size_t playerNb, size_t lifes);

    bool m_started = false;
};
} // namespace rtype::system
