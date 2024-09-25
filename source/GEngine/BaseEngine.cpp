/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** BaseEngine.cpp
*/

#include "GEngine/BaseEngine.hpp"
#include "GEngine/libdev/systems/events/Native.hpp"

namespace gengine {
void BaseEngine::compute(void) {
    while (m_ecs.hasEvent())
        m_ecs.executeEvent();
}

void BaseEngine::start(void) { m_ecs.publishEvent(system::event::StartEngine()); }
} // namespace gengine
