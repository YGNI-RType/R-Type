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

#include "components/Background.hpp"
#include "components/Monster.hpp"
#include "components/Player.hpp"
#include "components/Score.hpp"
#include "components/ScoreText.hpp"

#include <random>

namespace rtype {
void system::Start::init(void) {
    subscribeToEvent<gengine::system::event::StartEngine>(&Start::onStartEngine);
    subscribeToEvent<gengine::interface::event::NewRemoteDriver>(&Start::onNewRemoteDriver);
    subscribeToEvent<gengine::interface::event::DeleteRemoteDriver>(&Start::onDeleteRemoteDriver);
}

void system::Start::onStartEngine(gengine::system::event::StartEngine &e) {
    spawnEntity(component::ScoreText(), gengine::component::Transform2D({50, 50}, {5, 4}),
                gengine::component::driver::output::Drawable(2),
                gengine::component::driver::output::Text("arial.ttf", "Score: 0", WHITE));
                gengine::component::driver::output::Sprite("r-typesheet0.png", Rectangle{0, 0, 1120, 207}, WHITE));

                spawnEntity(
                    component::Background(), gengine::component::Transform2D({1119 * 3.48, 0}, {3.48, 3.48}),
                    gengine::component::Velocity2D(-DEFAULT_BACKGROUND_SPEED, 0),
                    gengine::component::driver::output::Drawable(0),
                    gengine::component::driver::output::Sprite("r-typesheet0.png", Rectangle{0, 0, 1120, 207}, WHITE));
}

void system::Start::onNewRemoteDriver(gengine::interface::event::NewRemoteDriver &e) {
    spawnEntity(
        component::Player(), gengine::component::Transform2D({0, static_cast<float>(rand() % 500)}, {3, 3}, 0),
        gengine::component::Velocity2D(0, 0), gengine::component::driver::output::Drawable(1),
        gengine::component::driver::output::Sprite("r-typesheet42.gif", Rectangle{66, 16.f * (m_nbPlayer % 5), 33, 17},
                                                   WHITE),
        gengine::component::HitBoxSquare2D(33 * 2, 17 * 2), gengine::interface::component::RemoteDriver(e.remote),
        gengine::component::driver::output::Animation("r-typesheet42.json/spaceship", 0.1f,
                                                      gengine::component::driver::output::AnimationTrack::Idling, 2),
        component::Score(0));
    m_nbPlayer++;
}

void system::Start::onDeleteRemoteDriver(gengine::interface::event::DeleteRemoteDriver &e) {
    auto &remotes = getComponents<gengine::interface::component::RemoteDriver>();
    for (auto &[entity, remote] : remotes) {
        if (remote == e.remote) {
            killEntity(entity);
            m_nbPlayer--;
            return;
        }
    }
}
} // namespace rtype
