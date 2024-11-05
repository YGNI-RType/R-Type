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
#include "components/Boss.hpp"
#include "components/Bullet.hpp"
#include "components/BulletEnemy.hpp"
#include "components/Monster.hpp"
#include "events/GameOver.hpp"
#include "events/NextStage.hpp"
#include "events/StartGame.hpp"
#include "systems/BossManager.hpp"
#include "systems/BulletManager.hpp"
#include "systems/MobManager.hpp"

#include <set>

namespace rtype::system {
class StageManager
    : public gengine::System<StageManager, MobManager, BulletManager, BossManager, component::Background,
                             component::Boss, component::Monster, component::Bullet, component::BulletEnemy,
                             geg::component::io::Sprite, geg::component::Velocity2D, geg::component::io::Drawable,
                             geg::component::Transform2D, geg::component::network::NetSend>,
      public gengine::RemoteSystem {
public:
    StageManager(const std::string &);
    void init(void) override;

    void onGameLoop(gengine::system::event::GameLoop &);
    void onStartEngine(gengine::system::event::StartEngine &);
    void onStartGame(event::StartGame &);

    void goToNextStage(event::NextStage &);
    void goToLobby(event::GoToLobby &);

private:
    void loadStages(void);
    void clearEntities(void);
    void initStage(std::size_t);

    void updateMonstersSpawn(void);
    void updateBossSpawn(void);
    void updateAmmoSpawn(void);

    std::string m_folder;
    Stage m_currentStage;

    struct CompareByStageId {
        bool operator()(const Stage &a, const Stage &b) const {
            return a.id < b.id;
        }
    };
    std::vector<Stage> m_stages;
    std::size_t m_currentStageIdx;
    unsigned int m_clock;
};
} // namespace rtype::system
