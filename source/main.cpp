/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** main.cpp
*/

// TODO reorder include
#include "GEngine/driver/Engine.hpp"
#include "GEngine/game/Engine.hpp"
#include "GEngine/interface/network/Networked.hpp"

#include "GEngine/libdev/components/HitBoxs.hpp"
// #include "libdev/components/Positions.hpp"

#include "GEngine/libdev/systems/Collisions.hpp"
#include "GEngine/libdev/systems/MainLoop.hpp"

#include "GEngine/libdev/components/Velocities.hpp"
#include "GEngine/libdev/systems/Collisions.hpp"
#include "GEngine/libdev/systems/Logger.hpp"
// #include "libdev/components/Positions.hpp"

#include "GEngine/libdev/systems/Collisions.hpp"
#include "GEngine/libdev/systems/Logger.hpp"
#include "GEngine/libdev/systems/MainLoop.hpp"
#include "GEngine/libdev/systems/Motions.hpp"

#include "GEngine/libdev/systems/Motions.hpp"

#include "GEngine/libdev/systems/Collisions.hpp"
#include "GEngine/libdev/systems/Logger.hpp"
#include "GEngine/libdev/systems/MainLoop.hpp"
#include "GEngine/libdev/systems/Motions.hpp"

#include "GEngine/libdev/systems/events/Log.hpp"

#include "systems/AutoMotion.hpp"
#include "systems/Start.hpp"

#include "GEngine/libdev/systems/driver/input/KeyboardCatcher.hpp"
#include "GEngine/libdev/systems/driver/input/MouseCatcher.hpp"
#include "GEngine/libdev/systems/driver/output/Animate.hpp"
#include "GEngine/libdev/systems/driver/output/Draw.hpp"
#include "GEngine/libdev/systems/driver/output/RenderWindow.hpp"
#include "GEngine/libdev/systems/driver/output/TextureManager.hpp"

#include "GEngine/libdev/components/driver/output/Animation.hpp"
#include "GEngine/libdev/components/driver/output/Shape.hpp"

void registerComponents(gengine::game::Engine &gameEngine) {
    gameEngine.registerComponent<gengine::component::Transform2D>();
    gameEngine.registerComponent<gengine::component::Velocity2D>();
    gameEngine.registerComponent<gengine::component::driver::output::Animation>();
    gameEngine.registerComponent<gengine::component::driver::output::Drawable>();
    gameEngine.registerComponent<gengine::component::driver::output::Sprite>();
}

void registerSystems(gengine::game::Engine &gameEngine) {
    gameEngine.registerSystem<gengine::system::AutoMainLoop>();

    gameEngine.registerSystem<gengine::system::driver::output::RenderWindow>(1280, 720, "R-Type");
    gameEngine.registerSystem<gengine::system::driver::output::Draw2D>();
    gameEngine.registerSystem<gengine::system::driver::output::DrawSprite>();
    gameEngine.registerSystem<gengine::system::driver::output::Animate>();
    gameEngine.registerSystem<gengine::system::driver::output::TextureManager>("../assets/sprites");

    gameEngine.registerSystem<hagarioop::systems::Start>();
    gameEngine.registerSystem<gengine::system::Motion2D>();
    gameEngine.registerSystem<hagarioop::systems::AutoMotion>();
    gameEngine.registerSystem<gengine::system::driver::input::KeyboardCatcher>();
}

int main(void) {
    gengine::game::Engine gameEngine;
    gengine::driver::Engine driverEngine;
    gengine::interface::network::Networked interface(gameEngine, driverEngine);

    registerComponents(gameEngine);
    registerSystems(gameEngine);

    interface.run();
}
