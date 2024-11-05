/*
** EPITECH PROJECT, 2024
** GEngine
** File description:
** Scenes.hpp
*/

#pragma once

#include "systems/Scenes.hpp"

namespace rtype::system::gui {
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

class WinScreen : public gengine::System<WinScreen, gengine::component::gui::SceneMember,
                                         geg::component::gui::SelectButton, component::GameState> {
public:
    void init(void) override;

    void onUpdate(geg::event::GameLoop &);
    void onSpawn(gengine::system::event::gui::SpawnScene &);
    void onClear(gengine::system::event::gui::ClearScene &);

private:
    short m_sceneId = WIN;

    gengine::Entity m_restartButton;

    bool m_update = false;
};
} // namespace rtype::system::gui