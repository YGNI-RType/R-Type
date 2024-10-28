/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** PlanesWave.hpp
*/

#pragma once

#include "GEngine/libdev/Components.hpp"
#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/systems/events/GameLoop.hpp"
#include "components/Monster.hpp"
#include "components/Plane.hpp"
#include "components/Score.hpp"
#include "events/BossSpawnWaveEvent.hpp"

namespace rtype::system {
class PlanesWave
    : public gengine::System<PlanesWave, gengine::component::HitBoxSquare2D, gengine::component::Transform2D,
                             gengine::component::Velocity2D, gengine::component::driver::output::Animation,
                             gengine::component::driver::output::Drawable, gengine::component::driver::output::Sprite,
                             component::Monster, component::Plane>,
      public gengine::RemoteSystem {
public:
    void init(void) override;
    void spawnWave(event::BossSpawnWaveEvent &e);
};
} // namespace rtype::system
