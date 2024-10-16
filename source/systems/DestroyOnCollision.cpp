
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

namespace rtype::system {
void DestroyOnCollision::init(void) {
    subscribeToEvent<gengine::system::event::Collsion>(&DestroyOnCollision::destroyMonster);
    subscribeToEvent<gengine::system::event::Collsion>(&DestroyOnCollision::destroyPlayer);
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

void DestroyOnCollision::destroyMonster(gengine::system::event::Collsion &e) {
    auto &monsters = getComponents<component::Monster>();
    auto &bullets = getComponents<component::Bullet>();
    auto &transforms = getComponents<gengine::component::Transform2D>();

    for (auto [entity_monster, monster] : monsters) {
        for (auto [entity_bullet, bullet] : bullets) {
            if (e.entity1 == entity_monster && e.entity2 == entity_bullet) {
                if (transforms.contains(entity_monster))
                    spawnEntity(gengine::component::Transform2D(transforms.get(entity_monster).pos, {2, 2}),
                                gengine::component::driver::output::Drawable(3),
                                gengine::component::driver::output::Sprite("r-typesheet44.gif", {129, 0, 33, 33}),
                                gengine::component::driver::output::Animation("r-typesheet44.json/small", 0.06f),
                                gengine::component::SpanLife(0.42));

                claimScore(entity_monster, reinterpret_cast<const char *>(bullet.from));
                killEntity(entity_monster);
                if (!bullet.isBeam)
                    killEntity(entity_bullet);
                return;
            }
        }
    }
}

void DestroyOnCollision::destroyPlayer(gengine::system::event::Collsion &e) {
    auto &players = getComponents<component::Player>();
    auto &monsters = getComponents<component::Monster>();
    auto &transforms = getComponents<gengine::component::Transform2D>();

    for (auto [entity_player, player] : players) {
        for (auto [entity_monster, monster] : monsters) {
            if (e.entity1 == entity_player && e.entity2 == entity_monster) {
                if (transforms.contains(entity_player))
                    spawnEntity(gengine::component::Transform2D(transforms.get(entity_player).pos, {3, 3}),
                                gengine::component::driver::output::Drawable(3),
                                gengine::component::driver::output::Sprite("r-typesheet1.gif", {67, 342, 33, 30}),
                                gengine::component::driver::output::Animation("r-typesheet1.json/playerdeath", 0.06f),
                                gengine::component::SpanLife(0.42));
                killEntity(entity_player);
                return;
            }
        }
    }
}
} // namespace rtype::system
