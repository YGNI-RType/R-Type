/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** main.cpp
*/

#include "GEngine/GEngine.hpp"
#include "GEngine/interface/network/Networked.hpp"

std::string getIp(int argc, char **argv) {
    if (argc > 1)
        return std::string(argv[1]);
    else
        return "127.0.0.1";
}

int main(int argc, char **argv) {
    std::string ip = getIp(argc, argv);

    GEngine::init();
    gengine::interface::network::Networked interface(GEngine::getLocal(), GEngine::getRemote(), ip, 4243, true);

    interface.run();
}
