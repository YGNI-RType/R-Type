/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** StageManager.hpp
*/

#pragma once

#include "GEngine/libdev/Components.hpp"
#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/systems/events/GameLoop.hpp"
#include "GEngine/libdev/systems/events/Native.hpp"

#include "Types.hpp"
#include "components/Background.hpp"
#include "events/NextStage.hpp"
#include "systems/BossManager.hpp"
#include "systems/MobManager.hpp"

namespace rtype::system {
class StageManager
    : public gengine::System<StageManager, MobManager, BossManager, component::Background, geg::component::io::Sprite,
                             geg::component::Velocity2D, geg::component::io::Drawable, geg::component::Transform2D,
                             geg::component::network::NetSend>,
      public gengine::RemoteSystem {
public:
    StageManager(const std::string &);
    void init(void) override;

    void onGameLoop(gengine::system::event::GameLoop &);
    void onStartEngine(gengine::system::event::StartEngine &);

    void goToNextStage(event::NextStage &);

private:
    void loadStages(void);
    void start(std::size_t);
    void updateMonstersSpawn(void);
    void updateBossSpawn(void);

    std::string m_folder;
    std::vector<Stage> m_stages;
    std::size_t m_currentStage;
    unsigned int m_clock;
};
} // namespace rtype::system
