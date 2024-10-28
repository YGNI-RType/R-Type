/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** PlanesAutoMotion.hpp
*/

#pragma once

#include "GEngine/libdev/Components.hpp"
#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/systems/events/GameLoop.hpp"

#include "components/Plane.hpp"

namespace rtype::system {
class PlanesAutoMotion
    : public gengine::System<PlanesAutoMotion, geg::component::Velocity2D, geg::component::Transform2D,
                             component::Plane, geg::component::network::NetSend> {
public:
    void init(void) override;
    void onGameLoop(gengine::system::event::GameLoop &);
};
} // namespace rtype::system
