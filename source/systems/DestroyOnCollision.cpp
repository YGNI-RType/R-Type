
/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** DestroyOnCollision.cpp
*/

#include "systems/DestroyOnCollision.hpp"
#include "Constants.hpp"
#include "events/GameOver.hpp"
#include "events/NextStage.hpp"

namespace rtype::system {
void DestroyOnCollision::init(void) {
    subscribeToEvent<geg::event::Collision>(&DestroyOnCollision::destroyMonster);
    subscribeToEvent<geg::event::Collision>(&DestroyOnCollision::destroyPlayer);
}

void DestroyOnCollision::claimScore(gengine::Entity entity_monster, const char *forPlayerUuid) {
    auto &scores = getComponents<component::Score>();
    auto &players = getComponents<gengine::interface::component::RemoteLocal>();

    if (scores.contains(entity_monster)) {
        for (auto [entity_player, player] : players)
            if (std::strcmp(player.getUUIDString().c_str(), forPlayerUuid) == 0)
                scores.get(entity_player).score += scores.get(entity_monster).score;
    }
}

void DestroyOnCollision::updateBossSprite(gengine::Entity entity_monster, unsigned int currentLives) {
    auto &sprites = getComponents<geg::component::io::Sprite>();
    auto &bosses = getComponents<component::Boss>();

    if (!bosses.contains(entity_monster) || !sprites.contains(entity_monster))
        return;

    auto &boss = bosses.get(entity_monster);
    auto &sprite = sprites.get(entity_monster);

    if (currentLives == boss.startLives / 2 || currentLives == boss.startLives / 4)
        sprite.src.x += sprite.src.width * 2;
}

void DestroyOnCollision::destroyMonster(geg::event::Collision &e) {
    auto &monsters = getComponents<component::Monster>();
    auto &bosses = getComponents<component::Boss>();
    auto &bullets = getComponents<component::Bullet>();
    auto &transforms = getComponents<gengine::component::Transform2D>();

    for (auto &[entity_monster, monster] : monsters) {
        for (auto &[entity_bullet, bullet] : bullets) {
            if (e.entity1 == entity_monster && e.entity2 == entity_bullet) {
                if (!bullet.isBeam)
                    killEntity(entity_bullet);
                if (transforms.contains(entity_monster))
                    spawnExplosion(entity_monster);
                if (monster.lives > 1) {
                    monster.lives--;
                    updateBossSprite(entity_monster, monster.lives);
                    return;
                }

                claimScore(entity_monster, bullet.from.c_str());
                if (bosses.contains(entity_monster))
                    publishEvent(event::NextStage());
                killEntity(entity_monster);
                return;
            }
        }
    }
}

void DestroyOnCollision::spawnExplosion(gengine::Entity entity) {
    auto &transforms = getComponents<gengine::component::Transform2D>();
    auto &hitboxes = getComponents<gengine::component::HitBoxSquare2D>();

    gengine::Vect2 &monster_pos = transforms.get(entity).pos;
    gengine::Vect2 &monster_scale = transforms.get(entity).scale;
    float explosion_width = 33;
    float bullet_scale = 2;
    float bullet_center = explosion_width / 2 * bullet_scale;
    gengine::Vect2 center = {monster_pos.x - bullet_center + hitboxes.get(entity).width * monster_scale.x / 2,
                             monster_pos.y - bullet_center + hitboxes.get(entity).height * monster_scale.y / 2};

    spawnEntity(
        gengine::component::Transform2D(center, {bullet_scale, bullet_scale}),
        gengine::component::driver::output::Drawable(3),
        gengine::component::driver::output::Sprite("explosions.gif", {129, 0, explosion_width, explosion_width}),
        gengine::component::driver::output::Animation("explosions.json/small", 0.06f),
        gengine::component::SpanLife(0.42));
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
                playerHit(entityPlayer, player, transform, players.size());
                return;
            }
        }
        for (auto [entityBulletEnemy, bulletEnemy] : bulletsEnemy) {
            if ((e.entity1 == entityPlayer || e.entity2 == entityPlayer) &&
                (e.entity1 == entityBulletEnemy || e.entity2 == entityBulletEnemy)) {
                playerHit(entityPlayer, player, transform, players.size());
                return;
            }
        }
    }
}

void DestroyOnCollision::playerHit(gengine::Entity entity, component::Player &player,
                                   gengine::component::Transform2D &transform, size_t nbAlive) {
    spawnEntity(gengine::component::Transform2D(transform.pos, {3, 3}), gengine::component::driver::output::Drawable(3),
                gengine::component::driver::output::Sprite("effects.gif", {67, 342, 33, 30}),
                gengine::component::driver::output::Animation("effects.json/playerdeath", 0.06f),
                gengine::component::SpanLife(0.42));
    player.lifes--;

    auto &sprites = getComponents<geg::component::io::Sprite>();

    if (player.lifes > 0) {
        setComponent(entity, component::Invincible());
        transform.pos = {0, WINDOW_HEIGHT / 2 - (sprites.get(entity).src.height * transform.scale.y) / 2.0f};
    } else {
        killEntity(entity);
        if (nbAlive - 1 == 0)
            publishEvent(event::GameOver(false));
    }
}
} // namespace rtype::system
