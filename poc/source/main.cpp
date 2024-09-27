/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** main.cpp
*/

#include "game/Engine.hpp"
#include "driver/Engine.hpp"
#include "interface/Internal.hpp"

#include "libdev/components/HitBoxs.hpp"
#include "libdev/components/Positions.hpp"
#include "libdev/components/Motions.hpp"

#include "libdev/systems/Collisions.hpp"
#include "libdev/systems/Motions.hpp"
#include "libdev/systems/MainLoop.hpp"
#include "libdev/systems/Logger.hpp"

#include "libdev/systems/events/Log.hpp"

#include "systems/Start.hpp"
#include "systems/AutoMotion.hpp"

#include "libdev/systems/driver/output/RenderWindow.hpp"
#include "libdev/systems/driver/output/Draw.hpp"
#include "libdev/systems/driver/input/KeyboardCatcher.hpp"

#include "libdev/components/driver/output/Shape.hpp"
#include "libdev/components/driver/output/Color.hpp"

class Killer: public gengine::System<Killer, gengine::component::Position2D, gengine::component::driver::output::Color> {
public:
    void init(void) override {
        subscribeToEvent<gengine::system::event::Collsion>(&Killer::onCollision);
    }

    void onCollision(gengine::system::event::Collsion &e) {
        auto &colors = getComponent<gengine::component::driver::output::Color>();
        std::string logMessage = "Receive collision between (" + std::to_string(e.entity1) + ") and (" + std::to_string(e.entity2) + ").";

        publishEvent(gengine::system::event::Log(logMessage));
        if (e.entity1 && e.entity2)
            return;
        if (e.entity1)
            colors.get(e.entity1).color = GREEN;
        if (e.entity2)
            colors.get(e.entity2).color = GREEN;
    }
};

int main(void)
{
    gengine::game::Engine gameEngine;
    gengine::driver::Engine driverEngine;

    // Components
    gameEngine.registerComponent<gengine::component::HitBoxCircle2D>();
    gameEngine.registerComponent<gengine::component::HitBoxSquare2D>();
    gameEngine.registerComponent<gengine::component::Position2D>();
    gameEngine.registerComponent<gengine::component::Motion2D>();
    gameEngine.registerComponent<gengine::component::Origin2D>();
    gameEngine.registerComponent<gengine::component::driver::output::Window>();
    gameEngine.registerComponent<gengine::component::driver::output::Rectangle>();
    gameEngine.registerComponent<gengine::component::driver::output::Color>();

    // Systems
    // customs
    gameEngine.registerSystem<hagarioop::systems::Start>();
    gameEngine.registerSystem<hagarioop::systems::AutoMotion>();
    // gameEngine.registerSystem<LogCollision>();

    // libdev
    gameEngine.registerSystem<gengine::system::Motion2D>();
    gameEngine.registerSystem<gengine::system::Collision2D>();
    gameEngine.registerSystem<gengine::system::driver::output::RenderWindow>();
    gameEngine.registerSystem<gengine::system::driver::output::Draw>();
    gameEngine.registerSystem<gengine::system::AutoMainLoop>();
    gameEngine.registerSystem<gengine::system::driver::input::KeyboardCatcher>();
    gameEngine.registerSystem<Killer>();
    gameEngine.registerSystem<gengine::system::Logger>("ECS.log");


    gengine::interface::Internal interface(gameEngine, driverEngine);
    interface.run();
}