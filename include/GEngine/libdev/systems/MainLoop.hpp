/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** MainLoop.hpp
*/

#pragma once

#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/systems/events/MainLoop.hpp"
#include "GEngine/libdev/systems/events/Native.hpp"

namespace gengine::system {
class AutoMainLoop : public gengine::System<AutoMainLoop> {
public:
    void init(void) override {
        subscribeToEvent<gengine::system::event::StartEngine>(&AutoMainLoop::onStartEngine);
        subscribeToEvent<gengine::system::event::MainLoop>(&AutoMainLoop::onMainLoop);
        subscribeToEvent<gengine::system::event::StopMainLoop>(&AutoMainLoop::onStopMainLoop);
    }

    void onStartEngine(gengine::system::event::StartEngine &e) { publishEvent(gengine::system::event::MainLoop()); }

    void onMainLoop(gengine::system::event::MainLoop &e) {
        if (m_isRunning)
            publishEvent(e);
    }

    void onStopMainLoop(gengine::system::event::StopMainLoop &e) { m_isRunning = false; }

private:
    bool m_isRunning = true;
};
} // namespace gengine::system
