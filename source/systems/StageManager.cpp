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
    , m_currentStageIdx(0)
    , m_clock(0) {
}

void StageManager::init(void) {
    subscribeToEvent<gengine::system::event::StartEngine>(&StageManager::onStartEngine);
    subscribeToEvent<event::StartGame>(&StageManager::onStartGame);
    subscribeToEvent<gengine::system::event::GameLoop>(&StageManager::onGameLoop);
    subscribeToEvent<event::NextStage>(&StageManager::goToNextStage);
}

void StageManager::updateAmmoSpawn(void) {
    for (auto &ammo : m_currentStage.ammo) {
        if (m_clock < ammo.spawnDelay)
            continue;

        auto &bulletManager = getSystem<BulletManager>();
        bulletManager.spawn(ammo);
        ammo.spawnDelay = -1;
    }
}

void StageManager::updateMonstersSpawn(void) {
    for (auto &monster : m_currentStage.monsters) {
        if (monster.spawnDelay == -1)
            continue;
        if (m_clock < monster.spawnDelay)
            break;

        auto &mobManager = getSystem<MobManager>();
        mobManager.spawn(monster, m_currentStage.ammo);
        monster.spawnDelay = -1;
    }
}

void StageManager::updateBossSpawn(void) {
    auto &boss = m_currentStage.boss;
    if (m_clock < boss.spawnDelay)
        return;

    auto &bossManager = getSystem<BossManager>();
    bossManager.spawn(boss);
    boss.spawnDelay = -1;
}

void StageManager::onStartGame(event::StartGame &) {
    m_currentStageIdx = 1;
    initStage(m_currentStageIdx);
}

void StageManager::onGameLoop(gengine::system::event::GameLoop &e) {
    m_clock += e.deltaTime;

    updateMonstersSpawn();
    updateBossSpawn();
    updateAmmoSpawn();
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
    std::sort(m_stages.begin(), m_stages.end(), CompareByStageId());
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
    if (m_stages.size() <= stageNbr) {
        publishEvent(event::GameOver(true));
        return;
    }

    m_clock = 0;
    clearEntities();

    m_currentStage = m_stages[stageNbr];
    spawnEntity(component::Background(), m_currentStage.background.transform, m_currentStage.background.velocity,
                geg::component::io::Drawable(0), m_currentStage.background.sprite, geg::component::network::NetSend());

    geg::component::Transform2D transform2 = m_currentStage.background.transform;
    transform2.pos.x += (m_currentStage.background.sprite.src.width - 1) * transform2.scale.x;

    spawnEntity(component::Background(), transform2, m_currentStage.background.velocity,
                geg::component::io::Drawable(0), m_currentStage.background.sprite, geg::component::network::NetSend());

    publishEvent(gengine::system::event::driver::output::Music("stage" + std::to_string(stageNbr + 1) + ".mp3"));
}

void StageManager::onStartEngine(gengine::system::event::StartEngine &e) {
    loadStages();
    initStage(m_currentStageIdx);
}

void StageManager::goToNextStage(event::NextStage &e) {
    initStage(++m_currentStageIdx);
}

void StageManager::goToLobby(event::GoToLobby &e) {
    m_currentStageIdx = 0;
    initStage(m_currentStageIdx);
}
} // namespace rtype::system
