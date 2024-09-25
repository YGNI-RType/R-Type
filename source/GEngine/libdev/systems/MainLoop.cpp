/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** MainLoop.cpp
*/

#pragma once

#include "GEngine/libdev/systems/MainLoop.hpp"

namespace gengine::system {
void AutoMainLoop::init(void) {
    subscribeToEvent<gengine::system::event::StartEngine>(&AutoMainLoop::onStartEngine);
    subscribeToEvent<gengine::system::event::MainLoop>(&AutoMainLoop::onMainLoop);
    subscribeToEvent<gengine::system::event::StopMainLoop>(&AutoMainLoop::onStopMainLoop);
}

void AutoMainLoop::onStartEngine(gengine::system::event::StartEngine &e) {
    publishEvent(gengine::system::event::MainLoop());
}

void AutoMainLoop::onMainLoop(gengine::system::event::MainLoop &e) {
    if (m_isRunning)
        publishEvent(e);
}

void AutoMainLoop::onStopMainLoop(gengine::system::event::StopMainLoop &e) { m_isRunning = false; }

} // namespace gengine::system
