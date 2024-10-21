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

#include "GEngine/interface/events/RemoteEvent.hpp"
#include "GEngine/libdev/systems/driver/input/KeyboardCatcher.hpp"
#include "events/Shoot.hpp"

#include "components/BulletEnemy.hpp"
#include "components/Monster.hpp"
#include "components/Plane.hpp"

#include "events/EnemyShootEvent.hpp"

namespace rtype::system {

class EnemyShoot
    : public gengine::System<EnemyShoot, component::Monster, component::BulletEnemy, component::Plane,
                             gengine::interface::component::RemoteDriver, gengine::component::Transform2D> {
public:
    void init(void) override;
    void shoot(event::EnemyShootEvent &);
};
} // namespace rtype::system
