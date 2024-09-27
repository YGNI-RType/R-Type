/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Draw.cpp
*/

#include "GEngine/libdev/systems/driver/output/Draw.hpp"
#include <iostream>

namespace gengine::system::driver::output {
void Draw::init(void) { subscribeToEvent<gengine::system::event::Draw>(&Draw::onDraw); }

void Draw::onDraw(gengine::system::event::Draw &e) {
    auto &rectangles = getComponent<component::driver::output::Rectangle>();
    auto &colors = getComponent<component::driver::output::Color>();
    auto &positions = getComponent<gengine::component::Position2D>();

    BeginDrawing();
    ClearBackground(RAYWHITE);
    for (auto &[entity, rect] : rectangles)
        if (colors.contains(entity))
            DrawRectangle(positions.get(entity).x, positions.get(entity).y, rect.width, rect.height,
                          colors.get(entity).color);
    EndDrawing();
}
} // namespace gengine::system::driver::output
