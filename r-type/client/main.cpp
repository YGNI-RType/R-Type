/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** main.cpp
*/

#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/Component.hpp"
#include "GEngine/libdev/systems/events/Native.hpp"

#include <iostream>

template <typename T>
struct Life: public gengine::Component {
    T life;
    Life(T &life_unit): life(life_unit) {};
    Life(T &&life_unit): life(life_unit) {};

    Life<T> &operator++(void) {
        life++;
        return *this;
    }
};

struct StartSystem : public gengine::System<StartSystem> {
    void init(void) override { subscribeToEvent<gengine::system::event::StartEngine>(&StartSystem::onStart); }

    void onStart(gengine::system::event::StartEngine &e) {
        spawnEntity(Life<int>(1));
        publishEvent(gengine::system::event::MainLoop());
    }
};

struct AddSystem: public gengine::System<AddSystem, Life<int>> {
    void init(void) override { subscribeToEvent<gengine::system::event::MainLoop>(&AddSystem::onMainLoop); }

    void onMainLoop(gengine::system::event::MainLoop &event) {
        auto &lifes = getComponent<Life<int>>();
        for (auto &[entity, life]: lifes) {
            life.life++;
            std::cout << life.life << std::endl;
        }
    }
};

struct AutoMainLoop: public gengine::System<AutoMainLoop> {
    void init (void) override {subscribeToEvent<gengine::system::event::MainLoop>(&AutoMainLoop::onMainLoop);}

    void onMainLoop(gengine::system::event::MainLoop &e) {
        auto event = gengine::system::event::MainLoop();
        publishEvent(event);
    }
};

#include "GEngine/driver/Engine.hpp"
#include "GEngine/game/Engine.hpp"
#include "GEngine/interface/Internal.hpp"

int main(void) {
    gengine::game::Engine gameEngine;
    gengine::driver::Engine driverEngine;

    gameEngine.registerSystem<StartSystem>();
    gameEngine.registerSystem<AddSystem>();
    gameEngine.registerSystem<AutoMainLoop>();
    gameEngine.registerComponent<Life<int>>();
    gengine::interface::Internal interface(gameEngine, driverEngine);
    interface.run();
}
