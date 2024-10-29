/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** StageManager.hpp
*/

#pragma once

#include "GEngine/interface/events/SharedEvent.hpp"
#include "GEngine/libdev/Components.hpp"
#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/systems/events/GameLoop.hpp"
#include "GEngine/libdev/systems/events/Native.hpp"

#include "Types.hpp"
#include "components/Background.hpp"
#include "components/Boss.hpp"
#include "components/Bounding.hpp"
#include "components/Flapping.hpp"
#include "components/Monster.hpp"
#include "components/Score.hpp"
#include "components/Shooter.hpp"
#include "events/NextStage.hpp"
#include "systems/BossManager.hpp"
#include "systems/MobManager.hpp"

namespace rtype::system {
class StageManager
    : public gengine::System<StageManager, component::Background, component::Monster, component::Bounding,
                             component::Boss, component::Shooter, component::Flapping, geg::component::io::Sprite,
                             geg::component::io::Animation, geg::component::Velocity2D, geg::component::io::Drawable,
                             geg::component::Transform2D, component::Score, geg::component::network::NetSend,
                             MobManager, BossManager>,
      public gengine::RemoteSystem {
public:
    StageManager(const std::string &);
    void init(void) override;

    void onGameLoop(gengine::system::event::GameLoop &);
    void onStartEngine(gengine::system::event::StartEngine &);
    void goToNextStage(gengine::interface::event::SharedEvent<event::NextStage> &);

    std::size_t getLastEntity(void);
    void setAnimationComponent(const geg::component::io::Animation &);
    void setMotionComponent(TypeOfMotion);
    void setShooterComponent(bool);

private:
    void loadStages(void);
    void start(std::size_t);
    void spawnMobs(const Monster &);
    void spawnBosses(const Monster &);
    void spawnMonster(const Monster &);

    std::string m_folder;
    std::vector<Stage> m_stages;
    std::size_t m_currentStage;
    int m_clock;
};
} // namespace rtype::system
