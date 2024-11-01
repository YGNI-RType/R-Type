/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** EnemyShoot.cpp
*/

#include "systems/EnemyShoot.hpp"
#include "GEngine/libdev/Components.hpp"

#define BULLET_SPEED_ENEMY 5

namespace rtype::system {
void EnemyShoot::init(void) {
    subscribeToEvent<gengine::system::event::GameLoop>(&EnemyShoot::onGameLoop);
    subscribeToEvent<event::EnemyShootEvent>(&EnemyShoot::shoot);
}

// TODO remove shooter component
void EnemyShoot::onGameLoop(gengine::system::event::GameLoop &e) {
    auto &shooters = getComponents<component::Shooter>();
    auto &transforms = getComponents<gengine::component::Transform2D>();

    // TODO upgrade this
    for (auto [entity, transform, shooter] : gengine::Zip(transforms, shooters))
        if (transform.pos.x < 500 && transform.pos.x > 495)
            publishEvent(event::EnemyShootEvent(entity));
}

// TODO make a bullet manager
// TODO spread code in differents functions
void EnemyShoot::shoot(event::EnemyShootEvent &e) {
    auto &shooters = getComponents<component::Shooter>();
    auto &transforms = getComponents<geg::component::Transform2D>();
    auto &players = getComponents<gengine::interface::component::RemoteLocal>();
    auto &hitboxes = getComponents<geg::component::HitBoxSquare2D>();
    auto &bosses = getComponents<component::Boss>();

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(1, players.size());
    int randomPlayer = dist6(rng);
    for (auto [entity, player, playerTransform, playerHitbox] : gengine::Zip(players, transforms, hitboxes)) {
        if (randomPlayer > 1) {
            randomPlayer--;
            continue;
        }

        for (auto [planeEntity, shooter, planeTransform] : gengine::Zip(shooters, transforms)) {
            if (planeEntity != e.from)
                continue;
            geg::component::Velocity2D velocity = geg::component::Velocity2D(
                {playerTransform.pos.x + playerHitbox.width / 2 * playerTransform.scale.x - planeTransform.pos.x,
                 playerTransform.pos.y + playerHitbox.height / 2 * playerTransform.scale.y - planeTransform.pos.y});
            float norm = std::sqrt(std::pow(velocity.x, 2) + std::pow(velocity.y, 2));
            velocity.x = velocity.x / norm * BULLET_SPEED_ENEMY;
            velocity.y = velocity.y / norm * BULLET_SPEED_ENEMY;

            spawnEntity(component::BulletEnemy(),
                        geg::component::Transform2D({planeTransform.pos.x, planeTransform.pos.y}, {2, 2}),
                        geg::component::Velocity2D(velocity),
                        geg::component::io::Sprite("bullets.gif", Rectangle{136, 6, 17, 6}, WHITE),
                        geg::component::io::Animation("bullets.json/small", 0.1f), geg::component::io::Drawable(1),
                        geg::component::HitBoxSquare2D(7, 6), geg::component::network::NetSend());
        }

        for (auto [bossEntity, bossComp, bossTransform] : gengine::Zip(bosses, transforms)) {
            if (bossEntity != e.from)
                continue;
            const auto &bossManager = getSystem<BossManager>();
            const auto &boss = bossManager.get(bossComp.bossName.c_str());

            geg::component::Velocity2D velocity = geg::component::Velocity2D(
                {playerTransform.pos.x + playerHitbox.width / 2 * playerTransform.scale.x - bossTransform.pos.x,
                 playerTransform.pos.y + playerHitbox.height / 2 * playerTransform.scale.y - bossTransform.pos.y});
            float norm = std::sqrt(std::pow(velocity.x, 2) + std::pow(velocity.y, 2));
            velocity.x = velocity.x / norm * boss.ballSpeed;
            velocity.y = velocity.y / norm * boss.ballSpeed;

            spawnEntity(component::BulletEnemy(),
                        geg::component::Transform2D({bossTransform.pos.x, bossTransform.pos.y}, {3, 3}),
                        geg::component::Velocity2D(velocity),
                        geg::component::io::Sprite("boss-ball.png", Rectangle{0, 0, 17, 15}, WHITE),
                        geg::component::io::Animation("boss-ball.json/rolling", 0.1f), geg::component::io::Drawable(1),
                        geg::component::HitBoxSquare2D(15, 15), geg::component::network::NetSend());
        }
        break;
    }
}
} // namespace rtype::system
