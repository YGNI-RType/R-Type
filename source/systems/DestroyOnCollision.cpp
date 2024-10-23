
/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** DestroyOnCollision.cpp
*/

#include "systems/DestroyOnCollision.hpp"
#include <cstring>

#include "GEngine/libdev/components/SpanLife.hpp"
#include "GEngine/libdev/components/driver/output/Animation.hpp"
#include "GEngine/libdev/components/driver/output/Drawable.hpp"
#include "GEngine/libdev/components/driver/output/Sprite.hpp"

#include "components/Invincible.hpp"

namespace rtype::system {
void DestroyOnCollision::init(void) {
    subscribeToEvent<geg::event::Collision>(&DestroyOnCollision::destroyMonster);
    subscribeToEvent<geg::event::Collision>(&DestroyOnCollision::destroyPlayer);
}

void DestroyOnCollision::claimScore(ecs::entity::Entity entity_monster, const char *forPlayerUuid) {
    auto &scores = getComponents<component::Score>();
    auto &players = getComponents<gengine::interface::component::RemoteDriver>();

    if (scores.contains(entity_monster)) {
        for (auto [entity_player, player] : players)
            if (std::strcmp(player.getUUIDString().c_str(), forPlayerUuid) == 0)
                scores.get(entity_player).score += scores.get(entity_monster).score;
    }
}

void DestroyOnCollision::destroyMonster(geg::event::Collision &e) {
    auto &monsters = getComponents<component::Monster>();
    auto &bullets = getComponents<component::Bullet>();
    auto &transforms = getComponents<gengine::component::Transform2D>();
    auto &barriers = getComponents<component::Barriers>();

    for (auto [entity_monster, monster] : monsters) {
        if (barriers.contains(entity_monster))
            continue;
        for (auto [entity_bullet, bullet] : bullets) {
            if (e.entity1 == entity_monster && e.entity2 == entity_bullet) {
                if (transforms.contains(entity_monster))
                    spawnEntity(gengine::component::Transform2D(transforms.get(entity_monster).pos, {2, 2}),
                                gengine::component::driver::output::Drawable(3),
                                gengine::component::driver::output::Sprite("r-typesheet44.gif", {129, 0, 33, 33}),
                                gengine::component::driver::output::Animation("r-typesheet44.json/small", 0.06f),
                                gengine::component::SpanLife(0.42));

                claimScore(entity_monster, bullet.from.c_str());
                killEntity(entity_monster);
                if (!bullet.isBeam)
                    killEntity(entity_bullet);
                return;
            }
        }
    }
}

void DestroyOnCollision::destroyPlayer(geg::event::Collision &e) {
    auto &players = getComponents<component::Player>();
    auto &monsters = getComponents<component::Monster>();
    auto &bulletsEnemy = getComponents<component::BulletEnemy>();
    auto &transforms = getComponents<gengine::component::Transform2D>();
    auto &invincibles = getComponents<component::Invincible>();

    for (auto [entityPlayer, player, transform] : gengine::Zip(players, transforms)) {
        if (invincibles.contains(entityPlayer))
            continue;
        for (auto [entityMonster, monster] : monsters) {
            if ((e.entity1 == entityPlayer || e.entity2 == entityPlayer) &&
                (e.entity1 == entityMonster || e.entity2 == entityMonster)) {
                playerHit(entityPlayer, player, transform);
                return;
            }
        }
        for (auto [entityBulletEnemy, bulletEnemy] : bulletsEnemy) {
            if ((e.entity1 == entityPlayer || e.entity2 == entityPlayer) &&
                (e.entity1 == entityBulletEnemy || e.entity2 == entityBulletEnemy)) {
                playerHit(entityPlayer, player, transform);
                return;
            }
        }
    }
}

void DestroyOnCollision::playerHit(ecs::entity::Entity entity, component::Player &player,
                                   gengine::component::Transform2D &transform) {
    spawnEntity(gengine::component::Transform2D(transform.pos, {3, 3}), gengine::component::driver::output::Drawable(3),
                gengine::component::driver::output::Sprite("r-typesheet1.gif", {67, 342, 33, 30}),
                gengine::component::driver::output::Animation("r-typesheet1.json/playerdeath", 0.06f),
                gengine::component::SpanLife(0.42));
    player.life--;

    if (player.life > 0) {
        setComponent(entity, component::Invincible(3));
        transform.pos = {0, static_cast<float>(rand() % 500)};
        removeLife();
    } else {
        killEntity(entity);
        spawnEntity(gengine::component::Transform2D({1280 / 2 - 350, 720 / 2 - 100}, {8, 8}),
                    gengine::component::driver::output::Drawable(2),
                    gengine::component::driver::output::Text("arcade.ttf", "GAME OVER", YELLOW));
    }
}

void DestroyOnCollision::removeLife(void) {
    auto &lifes = getComponents<component::Life>();

    for (auto [entity, life] : lifes) {
        killEntity(entity);
        return;
    }
}
} // namespace rtype::system
