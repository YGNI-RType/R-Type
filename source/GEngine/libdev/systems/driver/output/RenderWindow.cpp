/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** RenderWindow.cpp
*/

#include "GEngine/libdev/systems/driver/output/RenderWindow.hpp"
#include <iostream>

namespace gengine::system::driver::output {
void RenderWindow::init(void) {
    subscribeToEvent<gengine::system::event::StartEngine>(&RenderWindow::onStartEngine);
    subscribeToEvent<gengine::system::event::MainLoop>(&RenderWindow::onMainLoop);
}

void RenderWindow::onStartEngine(gengine::system::event::StartEngine &e) {
    auto &windows = getComponent<gengine::component::driver::output::Window>();
    if (!windows.size())
        return;
    auto &[width, height, title] = (windows.begin())->second;
    SetTraceLogLevel(LOG_WARNING);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    InitWindow(width, height, title.c_str());
    SetWindowMonitor(0);
    SetTargetFPS(GetMonitorRefreshRate(0));
}

void RenderWindow::onMainLoop(gengine::system::event::MainLoop &e) {

    auto &windows = getComponent<gengine::component::driver::output::Window>();

    if (!windows.size()) {
        publishEvent(gengine::system::event::StopMainLoop());
        return;
    }

    auto &[entity, window] = *windows.begin();
    if (WindowShouldClose())
        killEntity(entity);
    else
        publishEvent(gengine::system::event::Draw());

    if (IsWindowResized()) {
        window.width = GetRenderWidth();
        window.height = GetRenderHeight();
    }
}
} // namespace gengine::system::driver::output
