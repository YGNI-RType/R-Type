/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** PlayerGui.hpp
*/

#pragma once

#include "GEngine/libdev/Components.hpp"
#include "components/Invincible.hpp"
#include "components/Life.hpp"
#include "components/Player.hpp"
#include "events/Shoot.hpp"
#include "events/GameInputs.hpp"

#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/systems/events/GameLoop.hpp"
#include "GEngine/interface/network/events/Connection.hpp"

#include "GEngine/interface/events/RemoteLocal.hpp"

namespace rtype::system {
class PlayerGui : public gengine::System<PlayerGui, gengine::interface::component::RemoteLocal, component::Player,
                                         component::Life, component::Invincible, geg::component::io::Sprite, geg::component::io::Rectangle>,
                  public gengine::LocalSystem {
public:
    void init(void) override;

    void onConnection(gengine::interface::network::event::ConnectToServer &);

    void displayLife(gengine::system::event::GameLoop &);
    void beamCharge(event::Shoot &);
    void displayInvincibility(gengine::system::event::GameLoop &);
    void setMe(gengine::interface::event::ItsMe &);

private:
    uuids::uuid m_me;

    gengine::Entity m_bar;

    size_t m_life = 1;
    bool m_dead = true;
};
} // namespace rtype::system
