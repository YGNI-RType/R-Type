/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Start.hpp
*/

#include "systems/Start.hpp"

#include "libdev/components/HitBoxs.hpp"
#include "libdev/components/Motions.hpp"
#include "libdev/components/Positions.hpp"

#include "libdev/components/driver/output/Window.hpp"

#include "libdev/components/driver/output/Color.hpp"
#include "libdev/components/driver/output/Shape.hpp"

#include <random>

namespace hagarioop::systems {
void Start::init(void) { subscribeToEvent<gengine::system::event::StartEngine>(&Start::onStartEngine); }

void Start::onStartEngine(gengine::system::event::StartEngine &e) {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> entitys_fork(1, 500);
    std::uniform_int_distribution<> positionx_fork(0, 1920);
    std::uniform_int_distribution<> positiony_fork(0, 1080);
    std::uniform_int_distribution<> side_fork(20, 100);
    std::uniform_int_distribution<> motion_fork(-50, 50);

    spawnEntity(gengine::component::Position2D(1920 / 2, 1080 / 2), gengine::component::Motion2D(0, 0),
                gengine::component::HitBoxSquare2D(100, 100), gengine::component::driver::output::Color(BLACK),
                gengine::component::driver::output::Rectangle(1920 / 2, 1920 / 2, 100, 100)

    );

    spawnEntity(gengine::component::driver::output::Window(1920, 1080, "Hagar I OOP"));

    for (std::uint8_t i = 0; i < entitys_fork(gen); i++) {
        int side = side_fork(gen);
        int x = positionx_fork(gen);
        int y = positiony_fork(gen);
        spawnEntity(gengine::component::Position2D(x, y),
                    gengine::component::Motion2D(motion_fork(gen), motion_fork(gen)),
                    gengine::component::HitBoxSquare2D(side, side), gengine::component::driver::output::Color(RED),
                    gengine::component::driver::output::Rectangle(x, y, side, side)

        );
    }
}
} // namespace hagarioop::systems