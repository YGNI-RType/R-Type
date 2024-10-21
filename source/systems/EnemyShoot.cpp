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
}

void EnemyShoot::shoot(event::EnemyShootEvent &e) {
    auto &planes = getComponents<component::Plane>();
    auto &transforms = getComponents<gengine::component::Transform2D>();
    auto &players = getComponents<gengine::interface::component::RemoteDriver>();

    std::cout << players.size() << std::endl;
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(1, players.size());
    int randomPlayer = dist6(rng);
    for (auto [entity, player, playerTransform] : gengine::Zip(players, transforms)) {
        if (randomPlayer == 1) {
            for (auto [planeEntity, plane, planeTransform] : gengine::Zip(planes, transforms)) {
                if (planeEntity != e.from)
                    continue;
                gengine::component::Velocity2D direction = gengine::component::Velocity2D(
                    {playerTransform.pos.x - planeTransform.pos.x, playerTransform.pos.y - planeTransform.pos.y});
                gengine::component::Velocity2D velocity(direction);
                velocity.x = velocity.x / sqrt(pow(velocity.x, 2) + pow(velocity.y, 2)) * BULLET_SPEED_ENEMY;
                velocity.y = velocity.y / sqrt(pow(velocity.x, 2) + pow(velocity.y, 2)) * BULLET_SPEED_ENEMY;
                float rotation = atan2(velocity.y, velocity.x) * 180 / M_PI;
                spawnEntity(
                    component::BulletEnemy(),
                    gengine::component::Transform2D({planeTransform.pos.x, planeTransform.pos.y}, {2, 2}, rotation),
                    gengine::component::Velocity2D(velocity),
                    gengine::component::driver::output::Sprite("r-typesheet1.gif", Rectangle{248, 85, 17, 12}, RED),
                    gengine::component::driver::output::Drawable(1), gengine::component::HitBoxSquare2D(17, 12));
            }
        }
        randomPlayer--;
    }
}

} // namespace rtype::system
