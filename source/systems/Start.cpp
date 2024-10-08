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

#include <random>

namespace hagarioop::systems {
void Start::init(void) {
    subscribeToEvent<gengine::system::event::StartEngine>(&Start::onStartEngine);
}

void Start::onStartEngine(gengine::system::event::StartEngine &e) {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> transformXFork(0, 1280);
    std::uniform_int_distribution<> transformYFork(0, 720);

    spawnEntity(gengine::component::Transform2D({0, 0}, {5, 5}, 0), gengine::component::Velocity2D(0, 0),
                gengine::component::driver::output::Drawable(0),
                gengine::component::driver::output::Sprite("r-typesheet1.gif", Rectangle{101, 0, 33, 17}, WHITE),
                gengine::component::driver::output::Animation(5, 0, 10.f, false));

    for (std::size_t i = 0; i < 5; i++) {
        float x = transformXFork(gen);
        float y = transformYFork(gen);

        spawnEntity(gengine::component::Transform2D({x, y}, {5, 5}, 0), gengine::component::Velocity2D(5, 5),
                    gengine::component::driver::output::Sprite("r-typesheet3.gif", Rectangle{0, 0, 17, 18}, WHITE),
                    gengine::component::driver::output::Animation(12, rand() % 12, 0.1f),
                    gengine::component::driver::output::Drawable(0));
    }
}
} // namespace hagarioop::systems
