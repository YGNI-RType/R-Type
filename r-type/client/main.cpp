/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** main.cpp
*/

#include "GEngine/driver/Engine.hpp"
#include "GEngine/net/net.hpp"

int main(void)
{
    Network::NET::init();

    // LOOP THE GAME
    // check each frame the commands : connect (via gui or console)

    while (1) {
        Network::NET::sleep(4000000);
    }

    // -> code/client/cl_main?c (CL_FRAME)
    // check timeout
    // send UDP packets
    // update screen  

    Network::NET::stop();
    return 0;
}