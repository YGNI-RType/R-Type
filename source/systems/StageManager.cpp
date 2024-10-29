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
    , m_currentStage(0) {
}

void StageManager::init(void) {
    subscribeToEvent<gengine::system::event::StartEngine>(&StageManager::onStartEngine);
    subscribeToEvent<gengine::system::event::GameLoop>(&StageManager::onGameLoop);
    subscribeToEvent<gengine::interface::event::SharedEvent<event::NextStage>>(&StageManager::goToNextStage);
}

std::size_t StageManager::getLastEntity(void) {
    auto &entities = getComponents<geg::component::network::NetSend>();
    auto it = --entities.end();
    return it->first;
}

void StageManager::setAnimationComponent(const geg::component::io::Animation &animation) {
    if (!animation.trackName.empty())
        setComponent(getLastEntity(), animation);
}

void StageManager::setMotionComponent(TypeOfMotion type) {
    switch (type) {
    case TypeOfMotion::FLAPPING:
        setComponent(getLastEntity(), component::Flapping());
        break;
    case TypeOfMotion::BOUNDING:
        setComponent(getLastEntity(), component::Bounding());
        break;
    default:
        break;
    }
}

void StageManager::setShooterComponent(bool isShooting) {
    if (isShooting)
        setComponent(getLastEntity(), component::Shooter());
}

void StageManager::spawnMobs(const Monster &monster) {
    const auto &mobManager = getSystem<MobManager>();
    auto mobs = mobManager.getMob(monster.mobName);

    for (auto &mob : mobs) {
        mob.transform.scale.x *= monster.scaleFactor;
        mob.transform.scale.y *= monster.scaleFactor;
        mob.transform.pos.x = WINDOW_WIDTH * monster.spawnFactor.x -
                              (mob.sprite.src.width * mob.transform.scale.x) / 2.0f +
                              WINDOW_WIDTH * mob.transform.pos.x;
        mob.transform.pos.y = WINDOW_HEIGHT * monster.spawnFactor.y -
                              (mob.sprite.src.height * mob.transform.scale.y) / 2.0f +
                              WINDOW_HEIGHT * mob.transform.pos.y;
        mob.velocity.x *= monster.speedFactor;
        mob.velocity.y *= monster.speedFactor;

        spawnEntity(component::Monster(monster.numberOfLifes), mob.transform, mob.velocity, mob.sprite,
                    geg::component::io::Drawable(1),
                    geg::component::HitBoxSquare2D(mob.sprite.src.width, mob.sprite.src.height),
                    geg::component::network::NetSend(), component::Score(monster.scoreGain));

        setAnimationComponent(mob.animation);
        setMotionComponent(mob.typeOfMotion);
        setShooterComponent(mob.isShooting);
    }
}

void StageManager::spawnBosses(const Monster &monster) {
    const auto &bossManager = getSystem<BossManager>();
    auto boss = bossManager.getBoss(monster.mobName);

    boss.transform.scale.x *= monster.scaleFactor;
    boss.transform.scale.y *= monster.scaleFactor;
    boss.transform.pos.x = WINDOW_WIDTH * monster.spawnFactor.x -
                           (boss.sprite.src.width * boss.transform.scale.x) / 2.0f +
                           WINDOW_WIDTH * boss.transform.pos.x;
    boss.transform.pos.y = WINDOW_HEIGHT * monster.spawnFactor.y -
                           (boss.sprite.src.height * boss.transform.scale.y) / 2.0f +
                           WINDOW_HEIGHT * boss.transform.pos.y;
    boss.velocity.x *= monster.speedFactor;
    boss.velocity.y *= monster.speedFactor;

    spawnEntity(component::Boss(monster.mobName), component::Monster(monster.numberOfLifes), boss.transform,
                boss.velocity, boss.sprite, geg::component::io::Drawable(1),
                geg::component::HitBoxSquare2D(boss.sprite.src.width, boss.sprite.src.height * 0.2),
                geg::component::network::NetSend(), component::Score(monster.scoreGain));
    setAnimationComponent(boss.animation);
}

void StageManager::spawnMonster(const Monster &monster) {
    if (monster.isBoss)
        spawnBosses(monster);
    else
        spawnMobs(monster);
}

void StageManager::onGameLoop(gengine::system::event::GameLoop &e) {
    m_clock += e.deltaTime;

    for (auto &monster : m_stages[m_currentStage].monsters)
        if (m_clock > monster.spawnDelay) {
            spawnMonster(monster);
            monster.spawnDelay = -1;
        } else if (monster.spawnDelay != -1)
            break;
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

void StageManager::start(std::size_t stageNbr) {
    if (m_stages.size() <= stageNbr)
        throw std::runtime_error("Stage " + std::to_string(stageNbr + 1) + " not found");

    auto &backgrounds = getComponents<component::Background>();
    for (auto [entity, background] : backgrounds)
        killEntity(entity);

    spawnEntity(component::Background(), m_stages[stageNbr].background.transform,
                m_stages[stageNbr].background.velocity, geg::component::io::Drawable(0),
                m_stages[stageNbr].background.sprite, geg::component::network::NetSend());

    geg::component::Transform2D transform2 = m_stages[stageNbr].background.transform;
    transform2.pos.x += (m_stages[stageNbr].background.sprite.src.width - 1) * transform2.scale.x;

    spawnEntity(component::Background(), transform2, m_stages[stageNbr].background.velocity,
                geg::component::io::Drawable(0), m_stages[stageNbr].background.sprite,
                geg::component::network::NetSend());
}

void StageManager::onStartEngine(gengine::system::event::StartEngine &e) {
    loadStages();
    start(m_currentStage);
}

void StageManager::goToNextStage(gengine::interface::event::SharedEvent<event::NextStage> &e) {
    start(++m_currentStage);
}
} // namespace rtype::system
