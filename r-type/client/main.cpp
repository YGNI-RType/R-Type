/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** main.cpp
*/

#include "GEngine/driver/Engine.hpp"
#include "GEngine/game/Engine.hpp"
#include "GEngine/interface/Internal.hpp"
#include "GEngine/libdev/systems/MainLoop.hpp"

#include "GEngine/net/net.hpp"

#ifdef _WIN32
// TEMP TEMP TEMP : place it elsewhere, reminescence of windows.h from socket apis
#undef interface
#endif

int main(void) {
    Network::NET::init();

    Network::NET::initClient();
    Network::NET::pingServers();

    Network::CLNetClient &client = Network::NET::getClient();

    while (1) {
        if (Network::NET::sleep(4000))
            continue;
        if (!client.isConnected()) {
            client.connectToServer(0);
        }
            
    }

    Network::NET::stop();
    gengine::game::Engine gameEngine;
    gengine::driver::Engine driverEngine;

    gameEngine.registerSystem<gengine::system::AutoMainLoop>();
    gengine::interface::Internal interface(gameEngine, driverEngine);
    interface.run();
}
