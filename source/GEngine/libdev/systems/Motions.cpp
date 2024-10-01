/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Motions.cpp
*/

#include "GEngine/libdev/systems/Motions.hpp"

namespace gengine::system {
void Motion2D::init(void) { subscribeToEvent<event::MainLoop>(&Motion2D::onMainLoop); }

void Motion2D::onMainLoop(event::MainLoop &e) {
    auto &transforms = getComponent<component::Transform2D>();
    auto &velocities = getComponent<component::Velocity2D>();

    for (auto [entity, tr, vel] : Zip(transforms, velocities)) {
        tr.pos.x += (vel.x * e.deltaTime);
        tr.pos.y += (vel.y * e.deltaTime);
    }
}

void Motion3D::init(void) { subscribeToEvent<event::MainLoop>(&Motion3D::onMainLoop); }
void Motion3D::onMainLoop(event::MainLoop &e) {
    auto &transforms = getComponent<component::Transform3D>();
    auto &velocities = getComponent<component::Velocity3D>();

    for (auto [entity, tr, vel] : Zip(transforms, velocities)) {
        tr.pos.x += (vel.x * e.deltaTime);
        tr.pos.y += (vel.y * e.deltaTime);
        tr.pos.z += (vel.z * e.deltaTime);
    }
}
} // namespace gengine::system
