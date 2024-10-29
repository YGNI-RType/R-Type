/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** MobManager.cpp
*/

#include "systems/MobManager.hpp"
#include "Constants.hpp"

#include <filesystem>
#include <fstream>
#include <random>

namespace rtype::system {
MobManager::MobManager(const std::string &folder)
    : m_folder(folder) {
}

void MobManager::init(void) {
    subscribeToEvent<gengine::system::event::StartEngine>(&MobManager::onStartEngine);
    subscribeToEvent<event::BossSpawnWave>(&MobManager::spawnMob);
}

void MobManager::onStartEngine(gengine::system::event::StartEngine &e) {
    for (const auto &entry : std::filesystem::recursive_directory_iterator(m_folder)) {
        if (!entry.is_regular_file())
            continue;

        std::string filePath = entry.path().string();
        std::ifstream file(filePath);
        if (!file.is_open())
            continue;

        nlohmann::json jsonData = nlohmann::json::parse(file);

        std::string mobName = entry.path().filename().string();
        m_mobs[mobName] = jsonData.get<std::vector<Mob>>();
    }
}

void MobManager::spawnMob(event::BossSpawnWave &e) {
    auto &stageManager = getSystem<StageManager>();
    auto mobs = getMob(e.waveName.c_str());

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> randomY(30, 70);
    float y = randomY(gen) / 100.0f;

    for (auto &mob : mobs) {
        mob.transform.pos.x = WINDOW_WIDTH * 1.05f - (mob.sprite.src.width * mob.transform.scale.x) / 2.0f +
                              WINDOW_WIDTH * mob.transform.pos.x;
        mob.transform.pos.y = WINDOW_HEIGHT * y - (mob.sprite.src.height * mob.transform.scale.y) / 2.0f +
                              WINDOW_HEIGHT * mob.transform.pos.y;

        spawnEntity(component::Monster(), mob.transform, mob.velocity, mob.sprite, geg::component::io::Drawable(1),
                    geg::component::HitBoxSquare2D(mob.sprite.src.width, mob.sprite.src.height),
                    geg::component::network::NetSend());

        stageManager.setAnimationComponent(mob.animation);
        stageManager.setMotionComponent(mob.typeOfMotion);
        stageManager.setShooterComponent(mob.isShooting);
    }
}

const std::vector<Mob> &MobManager::getMob(const std::string &mobName) const {
    return m_mobs.at(mobName);
}
} // namespace rtype::system
