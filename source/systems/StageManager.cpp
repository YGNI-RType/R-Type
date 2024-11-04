/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** StageManager.cpp
*/

#include "systems/StageManager.hpp"
#include "Constants.hpp"

#include <filesystem>
#include <fstream>

namespace rtype::system {
StageManager::StageManager(const std::string &folder)
    : m_folder(folder)
    , m_currentStage(0)
    , m_clock(0)
    , m_started(false) {
}

void StageManager::init(void) {
    subscribeToEvent<gengine::system::event::StartEngine>(&StageManager::onStartEngine);
    subscribeToEvent<event::StartGame>(&StageManager::onStartGame);
    subscribeToEvent<event::GameOver>(&StageManager::onGameOver);
    subscribeToEvent<gengine::system::event::GameLoop>(&StageManager::onGameLoop);
    subscribeToEvent<event::NextStage>(&StageManager::goToNextStage);
}

void StageManager::updateAmmoSpawn(void) {
    for (auto &ammo : m_stages[m_currentStage].ammo) {
        if (m_clock < ammo.spawnDelay)
            continue;

        auto &bulletManager = getSystem<BulletManager>();
        bulletManager.spawn(ammo);
        ammo.spawnDelay = -1;
    }
}

void StageManager::updateMonstersSpawn(void) {
    for (auto &monster : m_stages[m_currentStage].monsters) {
        if (monster.spawnDelay == -1)
            continue;
        if (m_clock < monster.spawnDelay)
            break;

        auto &mobManager = getSystem<MobManager>();
        mobManager.spawn(monster, m_stages[m_currentStage].ammo);
        monster.spawnDelay = -1;
    }
}

void StageManager::updateBossSpawn(void) {
    auto &boss = m_stages[m_currentStage].boss;
    if (m_clock < boss.spawnDelay)
        return;

    auto &bossManager = getSystem<BossManager>();
    bossManager.spawn(boss);
    boss.spawnDelay = -1;
}

void StageManager::onStartGame(event::StartGame &) {
    m_started = true;
    m_clock = 0;

    m_currentStage = 0;
    initStage(m_currentStage);
}

void StageManager::onGameLoop(gengine::system::event::GameLoop &e) {
    if (!m_started)
        return;
    m_clock += e.deltaTime;

    updateMonstersSpawn();
    updateBossSpawn();
    updateAmmoSpawn();
}

void StageManager::onGameOver(event::GameOver &) {
    m_started = false;
}

void StageManager::loadStages(void) {
    for (const auto &entry : std::filesystem::recursive_directory_iterator(m_folder)) {
        if (!entry.is_regular_file())
            continue;

        std::string filePath = entry.path().string();
        std::ifstream file(filePath);
        if (!file.is_open())
            continue;

        nlohmann::json jsonData = nlohmann::json::parse(file);

        Stage stage = jsonData.get<Stage>();
        std::sort(stage.monsters.begin(), stage.monsters.end(),
                  [](const Monster &a, const Monster &b) { return a.spawnDelay < b.spawnDelay; });

        m_stages.push_back(stage);
    }
}

void StageManager::clearEntities(void) {
    std::vector<gengine::Entity> entities;

    auto &backgrounds = getComponents<component::Background>();
    for (auto [entity, background] : backgrounds)
        entities.push_back(entity);

    auto &monsters = getComponents<component::Monster>();
    for (auto [entity, monster] : monsters)
        entities.push_back(entity);

    auto &bosses = getComponents<component::Boss>();
    for (auto [entity, boss] : bosses)
        entities.push_back(entity);

    auto &bullets = getComponents<component::Bullet>();
    for (auto [entity, bullet] : bullets)
        entities.push_back(entity);

    auto &bulletsEnnemy = getComponents<component::BulletEnemy>();
    for (auto [entity, bulletEnnemy] : bulletsEnnemy)
        entities.push_back(entity);

    for (auto &entity : entities)
        killEntity(entity);
}

void StageManager::initStage(std::size_t stageNbr) {
    if (m_stages.size() <= stageNbr)
        throw std::runtime_error("Stage " + std::to_string(stageNbr + 1) + " not found");

    m_clock = 0;
    clearEntities();

    const auto &stage = m_stages[stageNbr];
    spawnEntity(component::Background(), stage.background.transform, stage.background.velocity,
                geg::component::io::Drawable(0), stage.background.sprite, geg::component::network::NetSend());

    geg::component::Transform2D transform2 = stage.background.transform;
    transform2.pos.x += (stage.background.sprite.src.width - 1) * transform2.scale.x;

    spawnEntity(component::Background(), transform2, stage.background.velocity, geg::component::io::Drawable(0),
                stage.background.sprite, geg::component::network::NetSend());
}

void StageManager::onStartEngine(gengine::system::event::StartEngine &e) {
    loadStages();
    initStage(m_currentStage);
}

void StageManager::goToNextStage(event::NextStage &e) {
    initStage(++m_currentStage);
}
} // namespace rtype::system
