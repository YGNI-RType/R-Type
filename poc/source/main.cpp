/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** main.cpp
*/

#include "driver/Engine.hpp"
#include "game/Engine.hpp"
#include "game/Engine.hpp"
#include "interface/Internal.hpp"

#include "libdev/components/HitBoxs.hpp"
#include "libdev/components/Motions.hpp"
<<<<<<< HEAD
<<<<<<< HEAD
// #include "libdev/components/Positions.hpp"

#include "libdev/systems/Collisions.hpp"
#include "libdev/systems/MainLoop.hpp"
=======
#include "libdev/components/Positions.hpp"

#include "libdev/systems/Collisions.hpp"
>>>>>>> aa51246 (added libdev: Exceptions)
#include "libdev/systems/Logger.hpp"
=======
#include "libdev/components/Motions.hpp"
#include "libdev/components/Positions.hpp"

#include "libdev/systems/Collisions.hpp"
>>>>>>> d2e9736 (updated poc: Sprite and animation)
#include "libdev/systems/MainLoop.hpp"
#include "libdev/systems/Logger.hpp"

#include "libdev/systems/events/Log.hpp"
#include "libdev/systems/Motions.hpp"

#include "systems/AutoMotion.hpp"
#include "systems/Start.hpp"
#include "systems/Start.hpp"

#include "libdev/systems/driver/input/KeyboardCatcher.hpp"
<<<<<<< HEAD
<<<<<<< HEAD
#include "libdev/systems/driver/input/MouseCatcher.hpp"
#include "libdev/systems/driver/output/Animate.hpp"
#include "libdev/systems/driver/output/Draw.hpp"
#include "libdev/systems/driver/output/RenderWindow.hpp"
#include "libdev/systems/driver/output/TextureManager.hpp"

#include "libdev/components/driver/output/Animation.hpp"
#include "libdev/components/driver/output/Shape.hpp"

#include <random>

class Killer : public gengine::System<Killer, gengine::component::Transform2D, gengine::component::HitBoxSquare2D> {
public:
    void init(void) override {
        subscribeToEvent<gengine::system::event::Collsion>(&Killer::onCollision);
        subscribeToEvent<gengine::system::event::driver::input::Mouse_Left>(&Killer::onClickLeft);
    }

    void onClickLeft(gengine::system::driver::input::MouseLeftEvent &e) {
        auto &transforms = getComponent<gengine::component::Transform2D>();
        auto &hitboxes = getComponent<gengine::component::HitBoxSquare2D>();
        for (auto &[entity, transf] : transforms) {
            if (hitboxes.contains(entity)) {
                auto &hitbox = hitboxes.get(entity);
                if (e.cursorPos.x >= transf.pos.x && e.cursorPos.y >= transf.pos.y &&
                    e.cursorPos.x <= transf.pos.x + hitbox.width * transf.scale.x &&
                    e.cursorPos.y <= transf.pos.y + hitbox.height * transf.scale.y) {
                    killEntity(entity);
                }
            }
        }
    }
    void onCollision(gengine::system::event::Collsion &e) {
=======
=======
#include "libdev/systems/driver/input/MouseCatcher.hpp"
>>>>>>> d2e9736 (updated poc: Sprite and animation)
#include "libdev/systems/driver/output/Draw.hpp"
#include "libdev/systems/driver/output/Animate.hpp"
#include "libdev/systems/driver/output/TextureManager.hpp"
#include "libdev/systems/driver/output/RenderWindow.hpp"

#include "libdev/components/driver/output/Color.hpp"
#include "libdev/components/driver/output/Animation.hpp"
#include "libdev/components/driver/output/Shape.hpp"

<<<<<<< HEAD
struct ChangeColor : public gengine::OnEventSystem<ChangeColor, gengine::system::event::Collsion,
                                                   gengine::component::driver::output::Color> {
    void onEvent(gengine::system::event::Collsion &e) override {
>>>>>>> aa51246 (added libdev: Exceptions)
        auto &colors = getComponent<gengine::component::driver::output::Color>();
        std::string logMessage =
            "Receive collision between (" + std::to_string(e.entity1) + ") and (" + std::to_string(e.entity2) + ").";

        publishEvent(gengine::system::event::Log(logMessage));
        if (e.entity1 && e.entity2)
            return;
        if (e.entity1)
            colors.get(e.entity1).color = BLUE;
        if (e.entity2)
            colors.get(e.entity2).color = GREEN;
=======
#include <random>


// class Killer : public gengine::System<Killer, gengine::component::Transform2D,
                                      gengine::component::HitBoxSquare2D> {
// public:
//     void init(void) override {
//         subscribeToEvent<gengine::system::event::Collsion>(&Killer::onCollision);
        subscribeToEvent<gengine::system::event::driver::input::Mouse_Left>(&Killer::onClickLeft);
    }

    void onClickLeft(gengine::system::driver::input::MouseLeftEvent &e) {
        auto &transforms = getComponent<gengine::component::Transform2D>();
        auto &hitboxes = getComponent<gengine::component::HitBoxSquare2D>();
        for (auto &[entity, transf] : transforms) {
            if (hitboxes.contains(entity)) {
                auto &hitbox = hitboxes.get(entity);
                if (e.cursorPos.x >= transf.pos.x && e.cursorPos.y >= transf.pos.y &&
                    e.cursorPos.x <= transf.pos.x + hitbox.width * transf.scale.x && e.cursorPos.y <= transf.pos.y + hitbox.height * transf.scale.y) {
                    killEntity(entity);
                }
            }
        }

        // auto &colors = getComponent<gengine::component::driver::output::Color>();

        // for (auto &[_, color] : colors)
        //     color = e.state == gengine::system::driver::input::InputState::RELEASE ? BLUE : GREEN;
//     }

//     void onCollision(gengine::system::event::Collsion &e) {
//         auto &colors = getComponent<gengine::component::driver::output::Color>();
//         std::string logMessage = "Receive collision between (" + std::to_string(e.entity1) + ") and (" + std::to_string(e.entity2) + ").";

//         publishEvent(gengine::system::event::Log(logMessage));
//         if (e.entity1 && e.entity2)
//             return;
//         if (e.entity1)
//             colors.get(e.entity1).color = GREEN;
//         if (e.entity2)
//             colors.get(e.entity2).color = GREEN;
//     }
// };

struct Killer: public gengine::OnEventSystem<Killer, gengine::system::event::Collsion, gengine::component::driver::output::Color> {
    void onEvent(gengine::system::event::Collsion &e) override {
        // auto &colors = getComponent<gengine::component::driver::output::Color>();
        std::string logMessage = "Receive collision between (" + std::to_string(e.entity1) + ") and (" + std::to_string(e.entity2) + ").";

        publishEvent(gengine::system::event::Log(logMessage));
        // if (e.entity1 && e.entity2)
        //     return;
        // if (e.entity1)
        //     colors.get(e.entity1).color = BLUE;
        // if (e.entity2)
<<<<<<< HEAD
        //     colors.get(e.entity2).color = GREEN;
>>>>>>> d2e9736 (updated poc: Sprite and animation)
=======
        //     colors.get(e.entity2).color = BLUE;
>>>>>>> fa6eebf (added libdev: OnEventSystem)
    }
};

<<<<<<< HEAD
        // auto &colors = getComponent<gengine::component::driver::output::Color>();

        // for (auto &[_, color] : colors)
        //     color = e.state == gengine::system::driver::input::InputState::RELEASE ? BLUE : GREEN;
//     }

// };

// struct ChangeColor : public gengine::OnEventSystem<ChangeColor, gengine::system::event::Collsion,
//                                                    gengine::component::driver::output::Color> {
//     void onEvent(gengine::system::event::Collsion &e) override {
//         auto &colors = getComponent<gengine::component::driver::output::Color>();
//         std::string logMessage =
//             "Receive collision between (" + std::to_string(e.entity1) + ") and (" + std::to_string(e.entity2) + ").";

//         publishEvent(gengine::system::event::Log(logMessage));
//         // if (e.entity1 && e.entity2)
//         //     return;
//         // if (e.entity1)
//         //     colors.get(e.entity1).color = BLUE;
//         // if (e.entity2)
//         //     colors.get(e.entity2).color = BLUE;
//     }
// };

=======
>>>>>>> aa51246 (added libdev: Exceptions)
int main(void) {
int main(void) {
    gengine::game::Engine gameEngine;
    gengine::driver::Engine driverEngine;

    // Components
    gameEngine.registerComponent<gengine::component::HitBoxCircle2D>();
    gameEngine.registerComponent<gengine::component::HitBoxSquare2D>();
<<<<<<< HEAD
<<<<<<< HEAD
    gameEngine.registerComponent<gengine::component::Transform2D>();
    gameEngine.registerComponent<gengine::component::Motion2D>();
    // gameEngine.registerComponent<gengine::component::Origin2D>();
    gameEngine.registerComponent<gengine::component::driver::output::Animation>();
=======
    gameEngine.registerComponent<gengine::component::Position2D>();
    // gameEngine.registerComponent<gengine::component::Motion2D>();
    gameEngine.registerComponent<gengine::component::Origin2D>();
    gameEngine.registerComponent<gengine::component::driver::output::Window>();
>>>>>>> aa51246 (added libdev: Exceptions)
=======
    gameEngine.registerComponent<gengine::component::Transform2D>();
    gameEngine.registerComponent<gengine::component::Motion2D>();
    gameEngine.registerComponent<gengine::component::Origin2D>();
    gameEngine.registerComponent<gengine::component::driver::output::Animation>();
>>>>>>> d2e9736 (updated poc: Sprite and animation)
    gameEngine.registerComponent<gengine::component::driver::output::Rectangle>();
    gameEngine.registerComponent<gengine::component::driver::output::Circle>();
    gameEngine.registerComponent<gengine::component::driver::output::Sprite>();
    // gameEngine.registerComponent<gengine::component::driver::output::Color>();

    // Systems
    // customs
    gameEngine.registerSystem<gengine::system::driver::output::RenderWindow>(1080, 720, "hagar");
    gameEngine.registerSystem<gengine::system::driver::output::TextureManager>("../sprites");
    // gameEngine.registerSystem<hagarioop::systems::AutoMotion>();
    // gameEngine.registerSystem<LogCollision>();

    // libdev
    gameEngine.registerSystem<hagarioop::systems::Start>();
    gameEngine.registerSystem<gengine::system::Motion2D>();
    gameEngine.registerSystem<gengine::system::Collision2D>();
    gameEngine.registerSystem<gengine::system::driver::output::Draw2D>();
    gameEngine.registerSystem<gengine::system::driver::output::Animate>();
    gameEngine.registerSystem<gengine::system::AutoMainLoop>();
    gameEngine.registerSystem<gengine::system::driver::input::KeyboardCatcher>();
    gameEngine.registerSystem<gengine::system::driver::input::MouseCatcher>();
    // std::cout << "test" << std::endl;
    gameEngine.registerSystem<Killer>();
    gameEngine.registerSystem<gengine::system::Logger>("ECS.log");


    gengine::interface::Internal interface(gameEngine, driverEngine);
    interface.run();
}
