/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** BaseEngine.cpp
*/

#include "GEngine/BaseEngine.hpp"
#include "GEngine/libdev/systems/events/Native.hpp"
#include <chrono>
#include <thread>

namespace gengine {
void BaseEngine::compute(void) {
    while (m_ecs.hasEvent()) {
        m_ecs.executeEvent();
        // std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    m_ecs.publishEvent(system::event::StopEngine());
    m_ecs.executeEvent();
    // while (m_ecs.hasEvent()) {
    //     m_ecs.executeEvent();
    //     // std::this_thread::sleep_for(std::chrono::milliseconds(100));
    // }
}

void BaseEngine::start(void) { m_ecs.publishEvent(system::event::StartEngine()); }
} // namespace gengine
