/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** main.cpp
*/

#include "GEngine/GEngine.hpp"
#include "GEngine/interface/network/Networked.hpp"
#include "GEngine/interface/network/Replay.hpp"

int main(int argc, const char **argv) {
    std::string replayFile;
    for (int i = 0; i < argc; i++) {
        if (!std::strcmp(argv[i], "--replay") && (i + 1) < argc)
            replayFile = argv[i + 1];
    }

    GEngine::init(argc, argv);
    if (replayFile.empty()) {
        gengine::interface::network::Networked interface(GEngine::getLocal(), GEngine::getRemote());
        interface.run();
    } else {
        gengine::interface::network::Replay interface(GEngine::getLocal(), GEngine::getRemote(), replayFile);
        interface.run();
    }

}
