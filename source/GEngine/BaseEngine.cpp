/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** BaseEngine.cpp
*/

#include "GEngine/BaseEngine.hpp"

namespace gengine {
void BaseEngine::update(void) { m_ecs.update(); }

void BaseEngine::start(void) { m_ecs.start(); }
} // namespace gengine
