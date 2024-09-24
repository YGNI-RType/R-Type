/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** main.cpp
*/

#include "GEngine/driver/Engine.hpp"
#include "GEngine/game/Engine.hpp"
#include "GEngine/interface/Internal.hpp"

struct Position {
    int x, y, z;
};

struct Motion {
    float x, y, z;
};

class StartSystem : public ecs::system::Base<StartSystem> {
public:
    StartSystem(const std::string &message) {
        std::cout << "Engine starting..., it say: \"" << message << "\"" << std::endl;
    }; // TODO args mut be const.

    void init(void) override { subscribeToEvent<ecs::system::event::StartEngine>(&StartSystem::onStart); }

    void onStart(ecs::system::event::StartEngine &event) { spawnEntity(Position{1, 1, 1}, Motion{2.f, 0.4, 1.5}); }
};

class MotionSystem : public ecs::system::Base<MotionSystem, Position, Motion> {
public:
    void init(void) override { subscribeToEvent<ecs::system::event::MainLoop>(&MotionSystem::mainLoop); }

    void mainLoop(ecs::system::event::MainLoop &event) {
        auto &positions = getComponent<Position>();
        auto &motions = getComponent<Motion>();

        for (auto &[entity, transform] : positions) {
            if (motions.contains(entity)) {
                auto &motion = motions.get(entity);
                transform.x += motion.x * 1.f;
                transform.y += motion.y * 1.f;
                std::cout << "Entity " << entity << " moved to (" << transform.x << ", " << transform.y << ")\n";
            }
        }
    }
};

#include "ecs/ECS.hpp"

int main(void) {
    gengine::driver::Engine DriverEngine;
    gengine::game::Engine GameEngine;
    GameEngine.registerComponent<Position>();
    GameEngine.registerComponent<Motion>();
    GameEngine.registerSystem<StartSystem>("Hello World !");
    GameEngine.registerSystem<MotionSystem>();

    gengine::interface::Internal interface(GameEngine, DriverEngine);
    interface.run();
}
