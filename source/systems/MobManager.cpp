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
    subscribeToEvent<event::BossSpawnWave>(&MobManager::spawn);
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

void MobManager::determineVelocity(const geg::component::Transform2D &mobTransform,
                                   const geg::component::io::Sprite &mobSprite,
                                   geg::component::Velocity2D &mobVelocity) {
    auto &transforms = getComponents<geg::component::Transform2D>();
    auto &sprites = getComponents<geg::component::io::Sprite>();
    auto &players = getComponents<gengine::interface::component::RemoteLocal>();

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(1, players.size());
    int randomPlayer = dist6(rng);

    for (auto [entity, player, playerTransform, playerSprite] : gengine::Zip(players, transforms, sprites)) {
        if (randomPlayer > 1) {
            randomPlayer--;
            continue;
        }

        geg::component::Velocity2D velocity(
            playerTransform.pos.x + (playerSprite.src.width * playerTransform.scale.x) / 2.0f - mobTransform.pos.x +
                (mobSprite.src.width * mobTransform.scale.x) / 2.0f,
            playerTransform.pos.y + (playerSprite.src.height * playerTransform.scale.y) / 2.0f - mobTransform.pos.y +
                (mobSprite.src.height * mobTransform.scale.y) / 2.0f);

        float norm = std::sqrt(std::pow(velocity.x, 2) + std::pow(velocity.y, 2));
        if (norm == 0)
            break;

        mobVelocity.x = velocity.x / norm * mobVelocity.x;
        mobVelocity.y = velocity.y / norm * mobVelocity.y;

        break;
    }
}

void MobManager::setMotionComponent(gengine::Entity entity, TypeOfMotion type) {
    switch (type) {
    case TypeOfMotion::FLAPPING:
        setComponent(entity, component::Flapping());
        break;
    case TypeOfMotion::BOUNDING:
        setComponent(entity, component::Bounding());
        break;
    default:
        break;
    }
}

void MobManager::spawn(const Monster &monster, std::vector<Ammo> &ammo) {
    auto mobs = get(monster.mobName);

    for (auto &mob : mobs) {
        mob.transform.scale.x *= monster.scaleFactor;
        mob.transform.scale.y *= monster.scaleFactor;
        mob.transform.pos.x = WINDOW_WIDTH * monster.spawnFactor.x -
                              (mob.sprite.src.width * mob.transform.scale.x) / 2.0f +
                              WINDOW_WIDTH * mob.transform.pos.x;
        mob.transform.pos.y = WINDOW_HEIGHT * monster.spawnFactor.y -
                              (mob.sprite.src.height * mob.transform.scale.y) / 2.0f +
                              WINDOW_HEIGHT * mob.transform.pos.y;

        if (mob.typeOfMotion == TypeOfMotion::FOLLOWING)
            determineVelocity(mob.transform, mob.sprite, mob.velocity);
        mob.velocity.x *= monster.speedFactor;
        mob.velocity.y *= monster.speedFactor;

        gengine::Entity entity = spawnEntity(component::Monster(monster.numberOfLives), mob.transform, mob.velocity,
                                             mob.sprite, geg::component::io::Drawable(1), mob.hitbox,
                                             geg::component::network::NetSend(), component::Score(monster.scoreGain));

        if (!mob.animation.trackName.empty())
            setComponent(entity, mob.animation);

        setMotionComponent(entity, mob.typeOfMotion);

        for (auto &a : mob.ammo) {
            a.spawnDelay /= monster.speedFactor;
            a.spawnDelay += monster.spawnDelay;
            a.mobId = entity;
            ammo.push_back(a);
        }
    }
}

void MobManager::spawn(event::BossSpawnWave &e) {
    auto mobs = get(e.waveName.c_str());

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> randomY(35, 65);
    float y = randomY(gen) / 100.0f;

    for (auto &mob : mobs) {
        mob.transform.pos.x = WINDOW_WIDTH * 1.05f - (mob.sprite.src.width * mob.transform.scale.x) / 2.0f +
                              WINDOW_WIDTH * mob.transform.pos.x;
        mob.transform.pos.y = WINDOW_HEIGHT * y - (mob.sprite.src.height * mob.transform.scale.y) / 2.0f +
                              WINDOW_HEIGHT * mob.transform.pos.y;

        if (mob.typeOfMotion == TypeOfMotion::FOLLOWING)
            determineVelocity(mob.transform, mob.sprite, mob.velocity);

        gengine::Entity entity =
            spawnEntity(component::Monster(), mob.transform, mob.velocity, mob.sprite, geg::component::io::Drawable(1),
                        mob.hitbox, geg::component::network::NetSend());

        if (!mob.animation.trackName.empty())
            setComponent(entity, mob.animation);

        setMotionComponent(entity, mob.typeOfMotion);
    }
}

const std::vector<Mob> &MobManager::get(const std::string &mobName) const {
    return m_mobs.at(mobName);
}
} // namespace rtype::system
