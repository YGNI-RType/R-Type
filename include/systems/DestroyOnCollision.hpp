/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** DestroyOnCollision.hpp
*/

#pragma once

#include "GEngine/interface/components/RemoteDriver.hpp"
#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/systems/events/game/Collision.hpp"

#include "GEngine/libdev/components/Transforms.hpp"
#include "components/Bullet.hpp"
#include "components/Monster.hpp"
#include "components/Player.hpp"
#include "components/Score.hpp"

#include "ecs/entity/Entity.hpp"

namespace rtype::system {
class DestroyOnCollision : public gengine::System<DestroyOnCollision, component::Bullet, component::Monster,
                                   component::Player, gengine::interface::component::RemoteDriver, gengine::component::Transform2D> {
public:
    void init(void) override;
    void destroyMonster(gengine::system::event::Collsion &);
    void destroyPlayer(gengine::system::event::Collsion &);

private:
    void claimScore(ecs::entity::Entity entity, const char *forPlayerUuid);
};
} // namespace rtype::system
