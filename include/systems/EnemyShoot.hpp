/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** EnemyShoot.hpp
*/

#pragma once

#include "GEngine/libdev/components/HitBoxs.hpp"
#include "GEngine/libdev/components/Transforms.hpp"

#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/systems/events/GameLoop.hpp"

#include "GEngine/interface/components/RemoteLocal.hpp"
#include "GEngine/libdev/systems/driver/input/KeyboardCatcher.hpp"
#include "events/Shoot.hpp"

#include "components/BulletEnemy.hpp"
#include "components/Monster.hpp"
#include "components/Plane.hpp"

#include "events/EnemyShootEvent.hpp"

namespace rtype::system {

class EnemyShoot : public gengine::System<EnemyShoot, component::Monster, component::BulletEnemy, component::Plane,
                                          gengine::interface::component::RemoteLocal, gengine::component::Transform2D,
                                          gengine::component::HitBoxSquare2D>, public gengine::RemoteSystem {
public:
    void init(void) override;
    void shoot(event::EnemyShootEvent &);
    // void rotateBullets(gengine::system::event::GameLoop &);
};
} // namespace rtype::system
