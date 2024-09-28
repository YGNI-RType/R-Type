/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Start.hpp
*/

#include "systems/Start.hpp"

#include "libdev/components/HitBoxs.hpp"
#include "libdev/components/Motions.hpp"
#include "libdev/components/Transforms.hpp"

#include "libdev/components/driver/output/Animation.hpp"
// #include "libdev/components/driver/output/Color.hpp"
#include "libdev/components/driver/output/Shape.hpp"
#include "libdev/components/driver/output/Sprite.hpp"

#include <random>

namespace hagarioop::systems {
void Start::init(void) { subscribeToEvent<gengine::system::event::StartEngine>(&Start::onStartEngine); }

void Start::onStartEngine(gengine::system::event::StartEngine &e) {
    auto &txtMan = getSystem<gengine::system::driver::output::TextureManager>();

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> entitys_fork(1, 500);
    std::uniform_int_distribution<> Transformx_fork(0, 1920);
    std::uniform_int_distribution<> Transformy_fork(0, 1080);
    std::uniform_int_distribution<> side_fork(20, 100);
    std::uniform_int_distribution<> motion_fork(-50, 50);

    spawnEntity(
        gengine::component::Transform2D({100, 100}, {5, 5}, 0), gengine::component::Motion2D(0, 0),
        gengine::component::HitBoxCircle2D(100),
        gengine::component::driver::output::Sprite(txtMan.get("r-typesheet1.gif"), Rectangle{101, 0, 33, 17}, WHITE),
        gengine::component::driver::output::Animation(5, 0, 0.1f));

    for (std::uint8_t i = 0; i < entitys_fork(gen); i++) {
        int side = side_fork(gen);
        float x = Transformx_fork(gen);
        float y = Transformy_fork(gen);
        spawnEntity(
            gengine::component::Transform2D({x, y}, {5, 5}, 0),
            gengine::component::Motion2D(motion_fork(gen), motion_fork(gen)),
            gengine::component::HitBoxSquare2D(side, side),
            // gengine::component::driver::output::Color(RED),
            // gengine::component::driver::output::Rectangle(side, side, RED)
            gengine::component::driver::output::Sprite(txtMan.get("r-typesheet3.gif"), Rectangle{0, 0, 17, 18}, WHITE),
            gengine::component::driver::output::Animation(12, rand() % 12, 0.1f, false)

        );
    }
}
} // namespace hagarioop::systems