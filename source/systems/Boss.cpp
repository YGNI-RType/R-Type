/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Boss.cpp
*/

#include "systems/Boss.hpp"

#include "Constants.hpp"
#include "events/BossSpawnWaveEvent.hpp"
#include "events/EnemyShootEvent.hpp"

#include <chrono>
#include <random>

namespace rtype::system {
void Boss::init(void) {
    subscribeToEvent<gengine::system::event::GameLoop>(&Boss::onGameLoop);
}

void Boss::onGameLoop(gengine::system::event::GameLoop &e) {
    std::random_device rd;
    std::mt19937 gen(rd());

    auto &motions = getComponents<geg::component::Velocity2D>();
    auto &transforms = getComponents<geg::component::Transform2D>();
    auto &bosses = getComponents<component::Boss>();
    auto &netsends = getComponents<geg::component::network::NetSend>();
    int playerCount = getComponents<gengine::interface::component::RemoteLocal>().size();

    for (auto [entity, motion, transform, boss, netsend] : gengine::Zip(motions, transforms, bosses, netsends)) {
        boss.waveSpawnInSec -= e.deltaTime / 1000.f;
        if (boss.waveSpawnInSec <= 0) {
            boss.waveSpawnInSec = boss.waveSpawnCooldown;
            publishEvent(event::BossSpawnWaveEvent(entity, boss.wavePlanesCount * playerCount));
        }
        std::uniform_int_distribution<> speed(2, boss.maxVelocity);
        if (transform.pos.y <= boss.borderMargin || transform.pos.y >= WINDOW_HEIGHT - boss.borderMargin) {
            motion.y = speed(gen);
            if (transform.pos.y >= WINDOW_HEIGHT - boss.borderMargin)
                motion.y = -motion.y;
            netsend.update();
            for (std::size_t i = 0; i < playerCount; i++)
                publishEvent(event::EnemyShootEvent(entity));
        }
    }
}
} // namespace rtype::system
