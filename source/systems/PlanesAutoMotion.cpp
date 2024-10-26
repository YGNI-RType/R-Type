/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** PlanesAutoMotion.cpp
*/

#include "systems/PlanesAutoMotion.hpp"

#include <random>

#include "events/EnemyShootEvent.hpp"

namespace rtype::system {
void PlanesAutoMotion::init(void) {
    subscribeToEvent<gengine::system::event::GameLoop>(&PlanesAutoMotion::onGameLoop);
}

void PlanesAutoMotion::onGameLoop(gengine::system::event::GameLoop &e) {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> change_chance_fork(0, 10);
    std::uniform_int_distribution<> motion_fork(-1, 1);

    auto &motions = getComponents<geg::component::Velocity2D>();
    auto &planes = getComponents<component::Plane>();
    auto &netsends = getComponents<geg::component::network::NetSend>();
    for (auto [entity, motion, plane, netsend] : gengine::Zip(motions, planes, netsends))
        if (change_chance_fork(gen) == 0) {
            motion.y = motion_fork(gen);
            netsend.update();
        }

    auto &transforms = getComponents<gengine::component::Transform2D>();
    for (auto [entity, transform, plane] : gengine::Zip(transforms, planes))
        if (transform.pos.x < 500 && transform.pos.x > 490)
            publishEvent(event::EnemyShootEvent(entity));
}
} // namespace rtype::system
