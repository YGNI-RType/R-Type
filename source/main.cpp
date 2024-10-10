/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** main.cpp
*/

// TODO reorder include

//? ## Engine ##
#include "GEngine/driver/Engine.hpp"
#include "GEngine/game/Engine.hpp"
#include "GEngine/interface/Internal.hpp"
// #include "GEngine/interface/network/Networked.hpp"

//? ## GEngine Components ##
#include "GEngine/libdev/components/HitBoxs.hpp"
#include "GEngine/libdev/components/Velocities.hpp"
#include "GEngine/libdev/components/driver/output/Animation.hpp"
#include "GEngine/libdev/components/driver/output/Shape.hpp"

//? ## GEngine Systems ##
#include "GEngine/libdev/systems/Collisions.hpp"
#include "GEngine/libdev/systems/MainLoop.hpp"
#include "GEngine/libdev/systems/Motions.hpp"
#include "GEngine/libdev/systems/driver/input/KeyboardCatcher.hpp"
#include "GEngine/libdev/systems/driver/output/Animate.hpp"
#include "GEngine/libdev/systems/driver/output/Draw.hpp"
#include "GEngine/libdev/systems/driver/output/RenderWindow.hpp"
#include "GEngine/libdev/systems/driver/output/TextureManager.hpp"

//? ### R-Type Components ###

#include "components/Background.hpp"
#include "components/Monster.hpp"
#include "components/Player.hpp"
#include "components/PlayerControl.hpp"

//? ### R-Type Systems ###
#include "systems/BackgroundMotion.hpp"
#include "systems/MonstersAutoMotion.hpp"
#include "systems/PlayerMotion.hpp"
#include "systems/PlayerShoot.hpp"
#include "systems/Start.hpp"

namespace rtype {
void registerComponents(gengine::game::Engine &gameEngine) {
    gameEngine.registerComponent<gengine::component::Transform2D>();
    gameEngine.registerComponent<gengine::component::Velocity2D>();
    gameEngine.registerComponent<gengine::component::driver::output::Animation>();
    gameEngine.registerComponent<gengine::component::driver::output::Drawable>();
    gameEngine.registerComponent<gengine::component::driver::output::Sprite>();
    gameEngine.registerComponent<gengine::component::driver::output::Rectangle>();

    gameEngine.registerComponent<component::Player>();
    gameEngine.registerComponent<component::PlayerControl>();
    gameEngine.registerComponent<component::Monster>();
    gameEngine.registerComponent<component::Background>();
}

void registerSystems(gengine::game::Engine &gameEngine) {
    gameEngine.registerSystem<gengine::system::AutoMainLoop>();

    gameEngine.registerSystem<gengine::system::driver::output::RenderWindow>(1280, 720, "R-Type");
    gameEngine.registerSystem<gengine::system::driver::output::Draw2D>();
    gameEngine.registerSystem<gengine::system::driver::output::DrawSprite>();
    gameEngine.registerSystem<gengine::system::driver::output::Animate>();
    gameEngine.registerSystem<gengine::system::driver::output::TextureManager>("../assets/sprites");
    gameEngine.registerSystem<gengine::system::driver::input::KeyboardCatcher>();
    gameEngine.registerSystem<gengine::system::Motion2D>();

    gameEngine.registerSystem<system::Start>();
    gameEngine.registerSystem<system::MonstersAutoMotion>();
    gameEngine.registerSystem<system::PlayerMotion>();
    gameEngine.registerSystem<system::PlayerShoot>();
    gameEngine.registerSystem<system::BackgroundMotion>();
}
} // namespace rtype

int main(void) {
    gengine::game::Engine gameEngine;
    gengine::driver::Engine driverEngine;
    // gengine::interface::network::Networked interface(gameEngine, driverEngine);
    gengine::interface::Internal interface(gameEngine, driverEngine);

    rtype::registerComponents(gameEngine);
    rtype::registerSystems(gameEngine);

    interface.run();
}
