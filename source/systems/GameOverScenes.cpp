/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** GameOverScenes.cpp
*/

#include "systems/GameOverScenes.hpp"

#include "Constants.hpp"
#include "events/StartGame.hpp"

namespace rtype::system::gui {
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

void WinScreen::init(void) {
    subscribeToEvent<geg::event::GameLoop>(&WinScreen::onUpdate);
    subscribeToEvent<gengine::system::event::gui::SpawnScene>(&WinScreen::onSpawn);
    subscribeToEvent<gengine::system::event::gui::ClearScene>(&WinScreen::onClear);
}

void WinScreen::onUpdate(geg::event::GameLoop &e) {
    if (!m_update)
        return;

    auto &buttons = getComponents<geg::component::gui::SelectButton>();

    bool ready = buttons.get(m_restartButton).state == geg::component::gui::SelectButton::PRESSED ? true : false;

    publishEvent(event::IAmReady(ready));
}

void WinScreen::onSpawn(gengine::system::event::gui::SpawnScene &e) {
    if (e.sceneId != m_sceneId)
        return;

    spawnEntity(gengine::component::gui::SceneMember(m_sceneId),
                gengine::component::Transform2D({WINDOW_WIDTH / 2 - 350, WINDOW_HEIGHT / 2 - 100}, {8, 8}),
                gengine::component::driver::output::Drawable(2),
                gengine::component::driver::output::Text("arcade.ttf", "YOU WON !", YELLOW));
    m_restartButton =
        spawnEntity(gengine::component::gui::SceneMember(m_sceneId), geg::component::io::Sprite("gui/Restart.png"),
                    geg::component::io::Drawable(10),
                    geg::component::Transform2D({WINDOW_WIDTH / 2 - 160, WINDOW_HEIGHT / 2 + 100}, {5, 5}),
                    gengine::component::gui::SelectButton(), gengine::component::gui::ButtonSpriteTint());

    m_update = true;
}

void WinScreen::onClear(gengine::system::event::gui::ClearScene &e) {
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
} // namespace rtype::system::gui