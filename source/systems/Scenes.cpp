/*
** EPITECH PROJECT, 2024
** GEngine
** File description:
** Scenes.cpp
*/

#include "systems/Scenes.hpp"
#include "Constants.hpp"
#include "components/InputBoxMapper.hpp"
#include "events/GameInputs.hpp"
#include "events/StartGame.hpp"

namespace rtype::system::gui {
void MainMenu::init(void) {
    subscribeToEvent<gengine::system::event::StartEngine>(&MainMenu::onStartEngine);
    subscribeToEvent<gengine::system::event::gui::ClearScene>(&MainMenu::onClear);
    subscribeToEvent<gengine::system::event::gui::SpawnScene>(&MainMenu::onSpawn);
}

void MainMenu::onStartEngine(gengine::system::event::StartEngine &e) {
    for (auto p = e.params.begin(); p != e.params.end(); p++) {
        if (*p == "--replay" && (p + 1) != e.params.end())
            publishEvent(gengine::system::event::gui::SwitchScene(RTYPE));
    }
}

void MainMenu::onSpawn(gengine::system::event::gui::SpawnScene &e) {
    if (e.sceneId != m_sceneId)
        return;
    spawnEntity(gengine::component::gui::SceneMember(m_sceneId), geg::component::io::Sprite("gui/MenuBackground.png"),
                geg::component::io::Drawable(10), geg::component::Transform2D({-50, -20}, {0.75, 0.75}));

    spawnEntity(
        gengine::component::gui::SceneMember(m_sceneId), geg::component::io::Sprite("gui/Multiplayer.png"),
        geg::component::io::Drawable(10), geg::component::Transform2D({WINDOW_WIDTH / 2 - 160, 300}, {5, 5}),
        gengine::component::gui::Button(), gengine::component::gui::ButtonSpriteTint(),
        gengine::component::gui::onClick([this] { publishEvent(gengine::system::event::gui::SwitchScene(SERVERS)); }));
    spawnEntity(
        gengine::component::gui::SceneMember(m_sceneId), geg::component::io::Sprite("gui/Settings.png"),
        geg::component::io::Drawable(10), geg::component::Transform2D({WINDOW_WIDTH / 2 - 160, 500}, {5, 5}),
        gengine::component::gui::Button(), gengine::component::gui::ButtonSpriteTint(),
        gengine::component::gui::onClick([this] { publishEvent(gengine::system::event::gui::SwitchScene(SETTINGS)); }));
}

void MainMenu::onClear(gengine::system::event::gui::ClearScene &e) {
    if (e.sceneId != m_sceneId)
        return;
    auto &members = getComponents<gengine::component::gui::SceneMember>();
    std::queue<ecs::entity::Entity> toKill;

    for (auto &[entity, member] : members)
        if (m_sceneId == member.sceneId)
            toKill.push(entity);
    while (!toKill.empty()) {
        killEntity(toKill.front());
        toKill.pop();
    }
}

Settings::Settings()
    : gengine::system::gui::BaseScene(SETTINGS) {
}

void Settings::onSpawn(gengine::system::event::gui::SpawnScene &e) {
    if (e.sceneId != m_sceneId)
        return;
    spawnEntity(gengine::component::gui::SceneMember(m_sceneId), geg::component::io::Sprite("gui/MenuBackground.png"),
                geg::component::io::Drawable(5), geg::component::Transform2D({-50, -20}, {0.75, 0.75}));
    spawnEntity(
        gengine::component::gui::SceneMember(m_sceneId), geg::component::io::Sprite("gui/Back.png"),
        geg::component::io::Drawable(10), geg::component::Transform2D({WINDOW_WIDTH / 2 - 160, 50}, {5, 5}),
        gengine::component::gui::Button(), gengine::component::gui::ButtonSpriteTint(),
        gengine::component::gui::onClick([this] { publishEvent(gengine::system::event::gui::SwitchScene(MAINMENU)); }));

    spawnEntity(gengine::component::gui::SceneMember(m_sceneId), geg::component::io::Sprite("gui/BigFrame.png"),
                geg::component::io::Drawable(9), geg::component::Transform2D({25, 200}, {7, 7}));

    spawnEntity(gengine::component::gui::SceneMember(m_sceneId),
                geg::component::io::Text("arcade.ttf", "EDIT CONTROL", RED), geg::component::io::Drawable(10),
                geg::component::Transform2D({135, 245}, {4, 4}));

    std::vector<std::string> titles = {"Move Up:", "Move Left:", "Move Down:", "Move Right:", "Shoot:"};
    std::vector<std::string> keys = {"UP", "LEFT", "DOWN", "RIGHT", "SPACE"};
    for (event::in::Input input = event::in::UP; input < event::in::CHEAT;
         input = static_cast<event::in::Input>(input + 1)) {

        spawnEntity(gengine::component::gui::SceneMember(m_sceneId),
                    geg::component::io::Text("arcade.ttf", titles[input], GREEN), geg::component::io::Drawable(10),
                    geg::component::Transform2D({145, 320 + static_cast<int>(input) * 60.f}, {3, 3}));
        gengine::Entity text =
            spawnEntity(gengine::component::gui::SceneMember(m_sceneId),
                        geg::component::io::Text("arcade.ttf", keys[input], WHITE), geg::component::io::Drawable(11),
                        geg::component::Transform2D({475, 320 + static_cast<int>(input) * 60.f}, {3, 3}));
        spawnEntity(gengine::component::gui::SceneMember(m_sceneId), geg::component::io::Sprite("gui/Edit.png"),
                    geg::component::io::Drawable(10),
                    geg::component::Transform2D({75, 315 + static_cast<int>(input) * 60.f}, {0.2, 0.2}),
                    gengine::component::gui::SelectButton(),
                    gengine::component::gui::ButtonSpriteTint(RAYWHITE, YELLOW, GRAY), geg::component::Box<1>({text}),
                    component::InputBoxMapper(input));
    }
    spawnEntity(gengine::component::gui::SceneMember(m_sceneId),
                geg::component::io::Text("arcade.ttf", "Font Size", PURPLE), geg::component::io::Drawable(11),
                geg::component::Transform2D({775, 300}, {4, 4}));
    spawnEntity(gengine::component::gui::SceneMember(m_sceneId), geg::component::io::Sprite("gui/Up.png"),
                geg::component::io::Drawable(10), geg::component::Transform2D({800, 375}, {0.5, 0.5}),
                gengine::component::gui::Button(), gengine::component::gui::ButtonSpriteTint(RAYWHITE, YELLOW, GRAY),
                gengine::component::gui::onClick([this] { publishEvent(event::ChangeTextSize(1)); }));
    spawnEntity(gengine::component::gui::SceneMember(m_sceneId), geg::component::io::Sprite("gui/Down.png"),
                geg::component::io::Drawable(10), geg::component::Transform2D({1000, 375}, {0.5, 0.5}),
                gengine::component::gui::Button(), gengine::component::gui::ButtonSpriteTint(RAYWHITE, YELLOW, GRAY),
                gengine::component::gui::onClick([this] { publishEvent(event::ChangeTextSize(-1)); }));
}

void Servers::init(void) {
    subscribeToEvent<geg::event::GameLoop>(&Servers::onUpdate);
    subscribeToEvent<gengine::system::event::gui::SpawnScene>(&Servers::onSpawn);
    subscribeToEvent<gengine::system::event::gui::ClearScene>(&Servers::onClear);
}

void Servers::onSpawn(gengine::system::event::gui::SpawnScene &e) {
    if (e.sceneId != m_sceneId)
        return;

    spawnEntity(gengine::component::gui::SceneMember(m_sceneId), geg::component::io::Sprite("gui/MenuBackground.png"),
                geg::component::io::Drawable(10), geg::component::Transform2D({-50, -20}, {0.75, 0.75}));
    spawnEntity(
        gengine::component::gui::SceneMember(m_sceneId), geg::component::io::Sprite("gui/Back.png"),
        geg::component::io::Drawable(10), geg::component::Transform2D({WINDOW_WIDTH / 2 - 480, 50}, {5, 5}),
        gengine::component::gui::Button(), gengine::component::gui::ButtonSpriteTint(),
        gengine::component::gui::onClick([this] { publishEvent(gengine::system::event::gui::SwitchScene(MAINMENU)); }));

    spawnEntity(gengine::component::gui::SceneMember(m_sceneId),
                geg::component::io::Text("arcade.ttf", "Enter IP", WHITE), geg::component::io::Drawable(10),
                geg::component::Transform2D({WINDOW_WIDTH / 2 - 480, 180}, {2, 2}));
    gengine::Entity text = spawnEntity(
        gengine::component::gui::SceneMember(m_sceneId), geg::component::io::Text("arcade.ttf", m_ip, WHITE),
        geg::component::io::Drawable(11), geg::component::Transform2D({WINDOW_WIDTH / 2 - 480 + 10, 215}, {3, 3}));
    spawnEntity(gengine::component::gui::SceneMember(m_sceneId), geg::component::io::Sprite("gui/InputBox.png"),
                geg::component::io::Drawable(10), geg::component::Transform2D({WINDOW_WIDTH / 2 - 480, 200}, {3, 3}),
                gengine::component::gui::SelectButton(),
                gengine::component::gui::ButtonSpriteTint(RAYWHITE, WHITE, GRAY), geg::component::Box<1>({text}),
                gengine::component::gui::InputBox(m_ip));

    spawnEntity(gengine::component::gui::SceneMember(m_sceneId), geg::component::io::Sprite("gui/Play.png"),
                geg::component::io::Drawable(10),
                geg::component::Transform2D({WINDOW_WIDTH / 2 + 140, 200}, {0.3, 0.3}),
                gengine::component::gui::Button(), gengine::component::gui::ButtonSpriteTint(WHITE, GRAY, RED),
                gengine::component::gui::onClick([this] {
                    if (m_ip.empty())
                        return;
                    publishEvent(gengine::interface::network::event::ConnectToServer(m_ip, 4243));
                    publishEvent(gengine::system::event::gui::SwitchScene(GAMELOBBY));
                }));
    spawnEntity(gengine::component::gui::SceneMember(m_sceneId),
                geg::component::io::Text("arcade.ttf", "LAN Servers", WHITE), geg::component::io::Drawable(10),
                geg::component::Transform2D({WINDOW_WIDTH / 2 - 480, 325}, {3, 3}));
    spawnEntity(gengine::component::gui::SceneMember(m_sceneId), geg::component::io::Sprite("gui/Refresh.png"),
                geg::component::io::Drawable(10),
                geg::component::Transform2D({WINDOW_WIDTH / 2 - 140, 315}, {0.2, 0.2}),
                gengine::component::gui::Button(), gengine::component::gui::ButtonSpriteTint(WHITE, GRAY, RED),
                gengine::component::gui::onClick([this] {
                    publishEvent(gengine::interface::network::event::PingLan());
                    for (auto &[i, entities] : m_servers)
                        for (auto entity : entities)
                            killEntity(entity);
                    m_servers.clear();
                }));
    publishEvent(gengine::interface::network::event::PingLan());
    for (auto &[i, entities] : m_servers)
        for (auto entity : entities)
            killEntity(entity);
    m_servers.clear();
    m_update = true;
}

void Servers::onUpdate(geg::event::GameLoop &e) {
    if (!m_update)
        return;

    auto &server = getSystem<gengine::interface::network::system::ClientServer>();
    for (auto info : server.getPingInfos()) {
        if (m_servers.find(info.ip) != m_servers.end())
            continue;

        gengine::Entity bg =
            spawnEntity(gengine::component::gui::SceneMember(m_sceneId), geg::component::io::Sprite("gui/Frame.png"),
                        geg::component::io::Drawable(10),
                        geg::component::Transform2D(
                            {50.f + 380.f * (m_servers.size() % 3), 380.f + m_servers.size() / 3 * 180.f}, {3.7, 3.7}));

        gengine::Entity text = spawnEntity(
            gengine::component::gui::SceneMember(m_sceneId),
            geg::component::io::Text("arcade.ttf", info.ip + "\n\n\t:" + std::to_string(info.port) +
                                                       "\n\n\nPlayers:\n\n\t" + std::to_string(info.currentPlayers) +
                                                       '/' + std::to_string(info.maxPlayers)),
            geg::component::io::Drawable(10),
            geg::component::Transform2D({75.f + 380.f * (m_servers.size() % 3), 405.f + m_servers.size() / 3 * 180.f},
                                        {2, 2}));

        gengine::Entity play =
            spawnEntity(gengine::component::gui::SceneMember(m_sceneId), geg::component::io::Sprite("gui/Play.png"),
                        geg::component::io::Drawable(10),
                        geg::component::Transform2D(
                            {330.f + 380.f * (m_servers.size() % 3), 470.f + m_servers.size() / 3 * 180.f}, {0.3, 0.3}),
                        gengine::component::gui::Button(), gengine::component::gui::ButtonSpriteTint(WHITE, GRAY, RED),
                        gengine::component::gui::onClick([this, info] {
                            publishEvent(gengine::interface::network::event::ConnectToServer(info.ip, info.port));
                            publishEvent(gengine::system::event::gui::SwitchScene(GAMELOBBY));
                        }));
        std::vector<gengine::Entity> entities = {bg, text, play};
        m_servers.emplace(info.ip, entities);
    }
}

void Servers::onClear(gengine::system::event::gui::ClearScene &e) {
    if (e.sceneId != m_sceneId)
        return;
    auto &members = getComponents<gengine::component::gui::SceneMember>();
    std::queue<ecs::entity::Entity> toKill;

    for (auto &[entity, member] : members)
        if (m_sceneId == member.sceneId)
            toKill.push(entity);
    while (!toKill.empty()) {
        killEntity(toKill.front());
        toKill.pop();
    }

    m_update = false;
}

void GameLobby::init(void) {
    subscribeToEvent<geg::event::GameLoop>(&GameLobby::onUpdate);
    subscribeToEvent<gengine::system::event::gui::SpawnScene>(&GameLobby::onSpawn);
    subscribeToEvent<gengine::system::event::gui::ClearScene>(&GameLobby::onClear);
}

void GameLobby::onUpdate(geg::event::GameLoop &e) {
    if (!m_update)
        return;

    auto &buttons = getComponents<geg::component::gui::SelectButton>();

    bool ready = buttons.get(m_startButton).state == geg::component::gui::SelectButton::PRESSED ? true : false;

    publishEvent(event::IAmReady(ready));

    auto &states = getComponents<component::GameState>();
}

void GameLobby::onSpawn(gengine::system::event::gui::SpawnScene &e) {
    if (e.sceneId != m_sceneId)
        return;
    m_startButton =
        spawnEntity(gengine::component::gui::SceneMember(m_sceneId), geg::component::io::Sprite("gui/Start.png"),
                    geg::component::io::Drawable(10), geg::component::Transform2D({WINDOW_WIDTH / 2 - 480, 50}, {5, 5}),
                    gengine::component::gui::SelectButton(), gengine::component::gui::ButtonSpriteTint());

    m_update = true;
}

void GameLobby::onClear(gengine::system::event::gui::ClearScene &e) {
    if (e.sceneId != m_sceneId)
        return;
    auto &members = getComponents<gengine::component::gui::SceneMember>();
    std::queue<ecs::entity::Entity> toKill;

    for (auto &[entity, member] : members)
        if (m_sceneId == member.sceneId)
            toKill.push(entity);
    while (!toKill.empty()) {
        killEntity(toKill.front());
        toKill.pop();
    }

    m_update = false;
}

void GameOver::init(void) {
    subscribeToEvent<geg::event::GameLoop>(&GameOver::onUpdate);
    subscribeToEvent<gengine::system::event::gui::SpawnScene>(&GameOver::onSpawn);
    subscribeToEvent<gengine::system::event::gui::ClearScene>(&GameOver::onClear);
}

void GameOver::onUpdate(geg::event::GameLoop &e) {
    if (!m_update)
        return;

    auto &buttons = getComponents<geg::component::gui::SelectButton>();

    bool ready = buttons.get(m_restartButton).state == geg::component::gui::SelectButton::PRESSED ? true : false;

    publishEvent(event::IAmReady(ready));

    auto &states = getComponents<component::GameState>();
}

void GameOver::onSpawn(gengine::system::event::gui::SpawnScene &e) {
    if (e.sceneId != m_sceneId)
        return;

    spawnEntity(gengine::component::gui::SceneMember(m_sceneId),
                gengine::component::Transform2D({WINDOW_WIDTH / 2 - 350, WINDOW_HEIGHT / 2 - 100}, {8, 8}),
                gengine::component::driver::output::Drawable(2),
                gengine::component::driver::output::Text("arcade.ttf", "GAME OVER", YELLOW));
    m_restartButton =
        spawnEntity(gengine::component::gui::SceneMember(m_sceneId), geg::component::io::Sprite("gui/Restart.png"),
                    geg::component::io::Drawable(10),
                    geg::component::Transform2D({WINDOW_WIDTH / 2 - 160, WINDOW_HEIGHT / 2 + 100}, {5, 5}),
                    gengine::component::gui::SelectButton(), gengine::component::gui::ButtonSpriteTint());

    m_update = true;
}

void GameOver::onClear(gengine::system::event::gui::ClearScene &e) {
    if (e.sceneId != m_sceneId)
        return;
    auto &members = getComponents<gengine::component::gui::SceneMember>();
    std::queue<ecs::entity::Entity> toKill;

    for (auto &[entity, member] : members)
        if (m_sceneId == member.sceneId)
            toKill.push(entity);
    while (!toKill.empty()) {
        killEntity(toKill.front());
        toKill.pop();
    }

    m_update = false;
}

void GameStateHandler::onEvent(geg::event::GameLoop &e) {
    auto &states = getComponents<component::GameState>();
    for (auto [entity, state] : states) {
        if (state.state == m_currentState)
            return;
        switch (state.state) {
        case component::GameState::LOBBY:
            publishEvent(gengine::system::event::gui::SwitchScene(GAMELOBBY));
            break;
        case component::GameState::GAME:
            publishEvent(gengine::system::event::gui::SwitchScene(RTYPE));
            break;
        case component::GameState::GAMEOVER:
            publishEvent(gengine::system::event::gui::SwitchScene(GAMEOVER));
            break;
        default:
            break;
        }
        m_currentState = state.state;
    }
}

void TextSizeModifier::onEvent(event::ChangeTextSize &e) {
    auto &texts = getComponents<geg::component::io::Text>();

    for (auto &[entity, text] : texts)
        text.fontSize += e.diff;
}
} // namespace rtype::system::gui