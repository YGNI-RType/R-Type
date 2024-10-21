/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** EnemyShoot.cpp
*/

#include "systems/EnemyShoot.hpp"

// Spawn entity
#include "GEngine/libdev/Component.hpp" // gengine::Zip
#include "GEngine/libdev/Components.hpp"

#define BULLET_SPEED_ENEMY 5

namespace rtype::system {
void EnemyShoot::init(void) {
    subscribeToEvent<event::EnemyShootEvent>(&EnemyShoot::shoot);
    // subscribeToEvent<gengine::system::event::GameLoop>(&EnemyShoot::rotateBullets);
}

void EnemyShoot::shoot(event::EnemyShootEvent &e) {
    auto &planes = getComponents<component::Plane>();
    auto &transforms = getComponents<geg::component::Transform2D>();
    auto &players = getComponents<gengine::interface::component::RemoteLocal>();
    auto &hitboxes = getComponents<geg::component::HitBoxSquare2D>();

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(1, players.size());
    int randomPlayer = dist6(rng);
    for (auto [entity, player, playerTransform, playerHitbox] : gengine::Zip(players, transforms, hitboxes)) {
        if (randomPlayer == 1) {
            for (auto [planeEntity, plane, planeTransform] : gengine::Zip(planes, transforms)) {
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
        }
        randomPlayer--;
    }
}
} // namespace rtype::system
