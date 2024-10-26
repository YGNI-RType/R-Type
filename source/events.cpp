/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** driver.cpp
*/

#include "GEngine/GEngine.hpp"

#include "GEngine/interface/network/systems/ClientEventPublisher.hpp"
#include "GEngine/interface/network/systems/ServerEventReceiver.hpp"

#include "events/Movement.hpp"
#include "events/Shoot.hpp"

void GEngineDeclareEvents(Registry *r) {
    // r->registerSystem<
    //     gengine::interface::network::system::ClientEventPublisher<
    //         rtype::event::Movement,
    //         rtype::event::Shoot
    //     >
    // >();
    // r->registerSystem<
    //     gengine::interface::network::system::ServerEventReceiver<
    //         rtype::event::Movement,
    //         rtype::event::Shoot
    //     >
    // >();
}
