/*
** EPITECH PROJECT, 2024
** GEngine
** File description:
** Scenes.hpp
*/

#pragma once

#include "GEngine/interface/network/events/Connection.hpp"
#include "GEngine/interface/network/systems/ClientServer.hpp"
#include "GEngine/libdev/Components.hpp"
#include "GEngine/libdev/Events.hpp"
#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/Systems.hpp"
#include "GEngine/libdev/systems/gui/BaseScene.hpp"

#include "GEngine/interface/components/RemoteLocal.hpp"
#include "components/GameState.hpp"
#include "components/Player.hpp"
#include "events/TextSize.hpp"

namespace rtype::system::gui {
enum Scenes { MAINMENU, SERVERS, SETTINGS, GAMELOBBY, RTYPE, GAMEOVER };
class MainMenu : public gengine::System<MainMenu, gengine::component::gui::SceneMember> {
public:
    void init(void) override;

    void onStartEngine(gengine::system::event::StartEngine &);
    void onSpawn(gengine::system::event::gui::SpawnScene &);
    void onClear(gengine::system::event::gui::ClearScene &);

private:
    short m_sceneId = MAINMENU;
};

class Settings : public gengine::system::gui::BaseScene {
public:
    Settings();

    void onSpawn(gengine::system::event::gui::SpawnScene &e) final;
};

class Servers : public gengine::System<Servers, gengine::component::gui::SceneMember,
                                       gengine::interface::network::system::ClientServer> {
public:
    void init(void) override;

    void onUpdate(geg::event::GameLoop &e);
    void onSpawn(gengine::system::event::gui::SpawnScene &e);
    void onClear(gengine::system::event::gui::ClearScene &e);

private:
    short m_sceneId = SERVERS;

    std::string m_ip = "127.0.0.1";

    std::unordered_map<std::string, std::vector<gengine::Entity>> m_servers;

    bool m_update = false;
};

class GameLobby : public gengine::System<GameLobby, gengine::component::gui::SceneMember,
                                         geg::component::gui::SelectButton, component::GameState> {
public:
    void init(void) override;

    void onUpdate(geg::event::GameLoop &);
    void onSpawn(gengine::system::event::gui::SpawnScene &);
    void onClear(gengine::system::event::gui::ClearScene &);

private:
    short m_sceneId = GAMELOBBY;

    gengine::Entity m_startButton;

    bool m_update = false;
};

class GameOver : public gengine::System<GameOver, gengine::component::gui::SceneMember,
                                        geg::component::gui::SelectButton, component::GameState> {
public:
    void init(void) override;

    void onUpdate(geg::event::GameLoop &);
    void onSpawn(gengine::system::event::gui::SpawnScene &);
    void onClear(gengine::system::event::gui::ClearScene &);

private:
    short m_sceneId = GAMEOVER;

    gengine::Entity m_restartButton;

    bool m_update = false;
};

class GameStateHandler
    : public gengine::OnEventSystem<GameStateHandler, geg::event::GameLoop, gengine::component::gui::SceneMember,
                                    geg::component::gui::SelectButton, component::GameState> {
public:
    void onEvent(geg::event::GameLoop &) final;

private:
    component::GameState::State m_currentState = component::GameState::State::LOBBY;
};

class TextSizeModifier
    : public gengine::OnEventSystem<TextSizeModifier, event::ChangeTextSize, geg::component::io::Text> {
public:
    void onEvent(event::ChangeTextSize &) final;
};
} // namespace rtype::system::gui