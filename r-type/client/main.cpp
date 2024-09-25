/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** main.cpp
*/

#include "GEngine/driver/Engine.hpp"
#include "GEngine/game/Engine.hpp"
#include "GEngine/interface/Internal.hpp"
#include "GEngine/libdev/systems/MainLoop.hpp"

#include "GEngine/libdev/systems/events/Native.hpp"

#include "GEngine/libdev/System.hpp"

#include "GEngine/libdev/components/HitBoxs.hpp"
#include "GEngine/libdev/components/Motions.hpp"
#include "GEngine/libdev/components/Positions.hpp"
#include "GEngine/libdev/systems/Collisions.hpp"
#include "GEngine/libdev/systems/Motions.hpp"

class Start : public gengine::System<Start> {
public:
    void init(void) override { subscribeToEvent<gengine::system::event::StartEngine>(&Start::onStartEngine); }

    void onStartEngine(gengine::system::event::StartEngine &e) {
        spawnEntity(gengine::component::Position2D(0, 0), gengine::component::Motion2D(1, 0),
                    gengine::component::HitBoxSquare2D(1, 1), gengine::component::Origin2D(0, 0));
        spawnEntity(gengine::component::Position2D(10, 0), gengine::component::Motion2D(-1, 0),
                    gengine::component::HitBoxSquare2D(1, 1));
    }
};

#include "GEngine/libdev/systems/events/game/Collision.hpp"

class HandleCollision : public gengine::System<HandleCollision> {
public:
    void init(void) override { subscribeToEvent<gengine::system::event::Collsion>(&HandleCollision::onCollision); }

    void onCollision(gengine::system::event::Collsion &e) {
        std::cout << "collision " << e.entity1 << "<->" << e.entity2 << std::endl;
    }
};

int main(void) {
    gengine::game::Engine gameEngine;
    gengine::driver::Engine driverEngine;

    gameEngine.registerComponent<gengine::component::Position2D>();
    gameEngine.registerComponent<gengine::component::Motion2D>();
    gameEngine.registerComponent<gengine::component::HitBoxSquare2D>();
    gameEngine.registerComponent<gengine::component::HitBoxCircle2D>();
    gameEngine.registerComponent<gengine::component::Origin2D>();

    gameEngine.registerSystem<Start>();
    gameEngine.registerSystem<gengine::system::Motion2D>();
    gameEngine.registerSystem<gengine::system::Collision2D>();
    gameEngine.registerSystem<HandleCollision>();
    gameEngine.registerSystem<gengine::system::AutoMainLoop>();
    gengine::interface::Internal interface(gameEngine, driverEngine);
    interface.run();
}
