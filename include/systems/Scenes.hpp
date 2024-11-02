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
#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/Systems.hpp"
#include "GEngine/libdev/Events.hpp"
#include "GEngine/libdev/systems/gui/BaseScene.hpp"

namespace rtype::system::gui {
enum Scenes {
    MAINMENU,
    SERVERS,
    SETTINGS,
    RTYPE,
    GAMEOVER
};
class MainMenu : public gengine::system::gui::BaseScene {
public:
    MainMenu();

    void onSpawn(gengine::system::event::gui::SpawnScene &e) final;

};

class Settings : public gengine::system::gui::BaseScene {
public:
    Settings();

    void onSpawn(gengine::system::event::gui::SpawnScene &e) final;
};

class Servers : public gengine::System<Servers, gengine::component::gui::SceneMember, gengine::interface::network::system::ClientServer> {
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
} // namespace rtype::system::gui