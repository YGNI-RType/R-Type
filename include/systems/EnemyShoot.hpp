/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** EnemyShoot.hpp
*/

#pragma once

#include "GEngine/interface/components/RemoteLocal.hpp"
#include "GEngine/libdev/Components.hpp"
#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/systems/events/GameLoop.hpp"

#include "components/Boss.hpp"
#include "components/BulletEnemy.hpp"
#include "components/Monster.hpp"
#include "components/Shooter.hpp"
#include "events/EnemyShootEvent.hpp"
#include "events/Shoot.hpp"
#include "systems/BossManager.hpp"

namespace rtype::system {
class EnemyShoot : public gengine::System<EnemyShoot, component::Monster, component::BulletEnemy, component::Shooter,
                                          component::Boss, gengine::interface::component::RemoteLocal,
                                          geg::component::Transform2D, geg::component::HitBoxSquare2D, BossManager>,
                   public gengine::RemoteSystem {
public:
    void init(void) override;

    void onGameLoop(gengine::system::event::GameLoop &);
    void shoot(event::EnemyShootEvent &);
};
} // namespace rtype::system
