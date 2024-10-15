
/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** DestroyOnCollision.cpp
*/

#include "systems/DestroyOnCollision.hpp"

namespace rtype::system {
void DestroyOnCollision::init(void) {
    subscribeToEvent<gengine::system::event::Collsion>(&DestroyOnCollision::destroyMonster);
    subscribeToEvent<gengine::system::event::Collsion>(&DestroyOnCollision::destroyPlayer);
}

void DestroyOnCollision::destroyMonster(gengine::system::event::Collsion &e) {
    auto &monsters = getComponents<component::Monster>();
    auto &bullets = getComponents<component::Bullet>();

    for (auto [entity_monster, monster] : monsters) {
        for (auto [entity_bullet, bullet] : bullets) {
            if ((e.entity1 == entity_monster || e.entity2 == entity_monster) &&
                (e.entity1 == entity_bullet || e.entity2 == entity_bullet)) {
                killEntity(entity_monster);
                if (!bullet.isBeam)
                    killEntity(entity_bullet);
            }
        }
    }
}

void DestroyOnCollision::destroyPlayer(gengine::system::event::Collsion &e) {
    auto &players = getComponents<component::Player>();
    auto &monsters = getComponents<component::Monster>();

    for (auto [entity_player, player] : players) {
        for (auto [entity_monster, monster] : monsters) {
            if ((e.entity1 == entity_player || e.entity2 == entity_player) &&
                (e.entity1 == entity_monster || e.entity2 == entity_monster)) {
                // killEntity(entity_player);
            }
        }
    }
}
} // namespace rtype::system
