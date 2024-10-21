/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** EnemyShoot.cpp
*/

#include "systems/EnemyShoot.hpp"

// Spawn entity
#include "GEngine/libdev/Component.hpp" // gengine::Zip
#include "GEngine/libdev/components/SpanLife.hpp"
#include "GEngine/libdev/components/Velocities.hpp"
#include "GEngine/libdev/components/driver/output/Animation.hpp"
#include "GEngine/libdev/components/driver/output/Drawable.hpp"
#include "GEngine/libdev/components/driver/output/Shape.hpp"
#include "GEngine/libdev/components/driver/output/Sprite.hpp"

#define BULLET_SPEED_ENEMY 5

namespace rtype::system {
void EnemyShoot::init(void) {
    subscribeToEvent<event::EnemyShootEvent>(&EnemyShoot::shoot);
    subscribeToEvent<gengine::system::event::GameLoop>(&EnemyShoot::rotateBullets);
}

void EnemyShoot::shoot(event::EnemyShootEvent &e) {
    auto &planes = getComponents<component::Plane>();
    auto &transforms = getComponents<gengine::component::Transform2D>();
    auto &players = getComponents<gengine::interface::component::RemoteDriver>();
    auto &hitboxes = getComponents<gengine::component::HitBoxSquare2D>();

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(1, players.size());
    int randomPlayer = dist6(rng);
    //TODO Remove hitboxes if useless
    for (auto [entity, player, playerTransform, playerHitbox] : gengine::Zip(players, transforms, hitboxes)) {
        if (randomPlayer == 1) {
            for (auto [planeEntity, plane, planeTransform] : gengine::Zip(planes, transforms)) {
                if (planeEntity != e.from)
                    continue;
                gengine::component::Velocity2D velocity = gengine::component::Velocity2D(
                    // {playerTransform.pos.x + playerHitbox.width / 2 * playerTransform.scale.x - planeTransform.pos.x,
                    //  playerTransform.pos.y + playerHitbox.height / 2 * playerTransform.scale.y -
                    //  planeTransform.pos.y});
                    {playerTransform.pos.x - planeTransform.pos.x, playerTransform.pos.y - planeTransform.pos.y});
                velocity.x = velocity.x / sqrt(pow(velocity.x, 2) + pow(velocity.y, 2)) * BULLET_SPEED_ENEMY;
                velocity.y = velocity.y / sqrt(pow(velocity.x, 2) + pow(velocity.y, 2)) * BULLET_SPEED_ENEMY;
                float rotation = atan2(velocity.y, velocity.x) * 180 / M_PI;
                spawnEntity(
                    component::BulletEnemy(),
                    gengine::component::Transform2D({planeTransform.pos.x, planeTransform.pos.y}, {2, 2}, rotation),
                    gengine::component::Velocity2D(velocity),
                    gengine::component::driver::output::Sprite("r-typesheet43.gif", Rectangle{136, 6, 7, 6}, WHITE),
                    gengine::component::driver::output::Drawable(1), gengine::component::HitBoxSquare2D(7, 6));
            }
        }
        randomPlayer--;
    }
}

void EnemyShoot::rotateBullets(gengine::system::event::GameLoop &e) {
    auto &bullets = getComponents<component::BulletEnemy>();
    auto &transforms = getComponents<gengine::component::Transform2D>();

    // Can rotate be from center ?
    for (auto [entity, bullet, transform] : gengine::Zip(bullets, transforms))
        transform.rotation += 8;
}

} // namespace rtype::system
