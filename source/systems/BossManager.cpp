/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** BossManager.cpp
*/

#include "systems/BossManager.hpp"
#include "Constants.hpp"
#include "events/BossSpawnWave.hpp"
#include "events/EnemyShootEvent.hpp"

#include <filesystem>
#include <fstream>
#include <random>

namespace rtype::system {
BossManager::BossManager(const std::string &folder)
    : m_folder(folder) {
}

void BossManager::init(void) {
    subscribeToEvent<gengine::system::event::StartEngine>(&BossManager::onStartEngine);
    subscribeToEvent<gengine::system::event::GameLoop>(&BossManager::onGameLoop);
}

void BossManager::onGameLoop(gengine::system::event::GameLoop &e) {
    std::random_device rd;
    std::mt19937 gen(rd());

    auto &bosses = getComponents<component::Boss>();
    auto &motions = getComponents<geg::component::Velocity2D>();
    auto &transforms = getComponents<geg::component::Transform2D>();
    auto &sprites = getComponents<geg::component::io::Sprite>();
    auto &netsends = getComponents<geg::component::network::NetSend>();

    for (auto [entity, motion, transform, sprite, bossComp, netsend] :
         gengine::Zip(motions, transforms, sprites, bosses, netsends)) {
        const auto &boss = getBoss(bossComp.bossName.c_str());

        std::uniform_int_distribution<> randomIndex(0, boss.wavesName.size() - 1);
        bossComp.waveSpawnInSec -= e.deltaTime;
        if (bossComp.waveSpawnInSec < 0) {
            bossComp.waveSpawnInSec = boss.waveCooldown;
            publishEvent(event::BossSpawnWave(boss.wavesName[randomIndex(gen)]));
        }

        std::uniform_int_distribution<> speed(boss.minVelocity, boss.maxVelocity);
        if (transform.pos.y < boss.borderMargin * WINDOW_HEIGHT - (sprite.src.height * transform.scale.y) / 2.0f) {
            motion.y = speed(gen);
            netsend.update();
            publishEvent(event::EnemyShootEvent(entity));
        }
        if (transform.pos.y >
            WINDOW_HEIGHT - boss.borderMargin * WINDOW_HEIGHT - (sprite.src.height * transform.scale.y) / 2.0f) {
            motion.y = -speed(gen);
            netsend.update();
            publishEvent(event::EnemyShootEvent(entity));
        }
    }
}

void BossManager::onStartEngine(gengine::system::event::StartEngine &e) {
    for (const auto &entry : std::filesystem::recursive_directory_iterator(m_folder)) {
        if (!entry.is_regular_file())
            continue;

        std::string filePath = entry.path().string();
        std::ifstream file(filePath);
        if (!file.is_open())
            continue;

        nlohmann::json jsonData = nlohmann::json::parse(file);

        std::string bossName = entry.path().filename().string();
        m_bosses[bossName] = jsonData.get<Boss>();
    }
}

const Boss &BossManager::getBoss(const std::string &bossName) const {
    return m_bosses.at(bossName);
}
} // namespace rtype::system
