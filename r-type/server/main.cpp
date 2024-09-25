/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** main.cpp
*/

#include "GEngine/game/Engine.hpp"
#include "GEngine/net/net.hpp"

int main(void)
{
    Network::NET::init();

    while (1) {
        Network::NET::sleep(4000);
    }

    Network::NET::stop();
    return 0;
}
