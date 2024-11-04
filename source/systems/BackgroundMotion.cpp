/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** BackgroundAutoMotion.cpp
*/

#include "systems/BackgroundMotion.hpp"

namespace rtype::system {
void BackgroundMotion::init(void) {
    subscribeToEvent<gengine::system::event::GameLoop>(&BackgroundMotion::onGameLoop);
}

void BackgroundMotion::onGameLoop(gengine::system::event::GameLoop &e) {
    auto &backgrounds = getComponents<component::Background>();
    auto &transforms = getComponents<gengine::component::Transform2D>();
    auto &sprites = getComponents<gengine::component::driver::output::Sprite>();

    for (auto [entity, background, transform, sprite] : gengine::Zip(backgrounds, transforms, sprites))
        if (transform.pos.x < -(sprite.src.width - 1) * transform.scale.x - 1)
            transform.pos.x = (sprite.src.width - 1) * transform.scale.x;
}
} // namespace rtype::system
