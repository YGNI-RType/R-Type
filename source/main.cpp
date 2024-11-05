/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** main.cpp
*/

#include "GEngine/GEngine.hpp"
#include "GEngine/interface/network/Networked.hpp"
#include "GEngine/interface/network/Replay.hpp"

#include "GEngine/net/net_socket_error.hpp"
#include "exceptions/Base.hpp"

int main(int argc, const char **argv) {
    std::string replayFile;
    for (int i = 0; i < argc; i++)
        if (!std::strcmp(argv[i], "--replay") && (i + 1) < argc)
            replayFile = argv[i + 1];

    try {
        GEngine::init(argc, argv);
        if (replayFile.empty()) {
            gengine::interface::network::Networked interface(GEngine::getLocal(), GEngine::getRemote());
            interface.run();
        } else {
            gengine::interface::network::Replay interface(GEngine::getLocal(), GEngine::getRemote(), replayFile);
            interface.run();
        }
    } catch (gengine::Exception &e) {
        std::cout << e.what() << std::endl;
    } catch (Network::SocketException &e) {
        std::cout << e.what() << std::endl;
    } catch (Network::NetException &e) {
        std::cout << e.what() << std::endl;
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
}
