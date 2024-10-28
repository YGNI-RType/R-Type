/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Boss.hpp
*/

#pragma once

#include "GEngine/interface/components/RemoteLocal.hpp"
#include "GEngine/libdev/Components.hpp"
#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/systems/events/GameLoop.hpp"

#include "components/Boss.hpp"

namespace rtype::system {
class Boss : public gengine::System<Boss, geg::component::Velocity2D, geg::component::Transform2D,
                                          gengine::interface::component::RemoteLocal, component::Boss,
                                          geg::component::network::NetSend>,
                   public gengine::RemoteSystem {
public:
    void init(void) override;
    void onGameLoop(gengine::system::event::GameLoop &);
};
} // namespace rtype::system
