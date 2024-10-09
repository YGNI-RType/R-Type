/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Start.hpp
*/

#include "systems/Start.hpp"

#include "GEngine/libdev/components/HitBoxs.hpp"
#include "GEngine/libdev/components/Transforms.hpp"
#include "GEngine/libdev/components/Velocities.hpp"

#include "GEngine/libdev/components/driver/output/Animation.hpp"
#include "GEngine/libdev/components/driver/output/Drawable.hpp"
#include "GEngine/libdev/components/driver/output/Shape.hpp"
#include "GEngine/libdev/components/driver/output/Sprite.hpp"
#include "GEngine/libdev/components/driver/output/Text.hpp"

#include "components/Player.hpp"
#include "components/PlayerControl.hpp"

#include "components/Monster.hpp"

#include <random>

namespace rtype {
void system::Start::init(void) {
    subscribeToEvent<gengine::system::event::StartEngine>(&Start::onStartEngine);
}

void system::Start::onStartEngine(gengine::system::event::StartEngine &e) {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> transformXFork(0 + 80, 1280 - 160);
    std::uniform_int_distribution<> transformYFork(0 + 80, 720 - 160);

    for (std::size_t i = 0; i < 10; i++) {
        float x = transformXFork(gen);
        float y = transformYFork(gen);

        spawnEntity(gengine::component::Transform2D({x, y}, {2, 2}, 0), gengine::component::Velocity2D(0, 0),
                    gengine::component::driver::output::Sprite("r-typesheet3.gif", Rectangle{0, 0, 17, 18}, WHITE),
                    gengine::component::driver::output::Animation(12, rand() % 12, 0.1f),
                    gengine::component::driver::output::Drawable(0), component::Monster("poulet"));
    }

    spawnEntity(component::Player("Arcod"), component::PlayerControl(),
                gengine::component::Transform2D({0, 0}, {3, 3}, 0), gengine::component::Velocity2D(0, 0),
                gengine::component::driver::output::Drawable(0),
                gengine::component::driver::output::Sprite("r-typesheet1.gif", Rectangle{101, 0, 33, 17}, WHITE),
                gengine::component::driver::output::Animation(5, 0, 0.2f, true));
}
} // namespace rtype::system
