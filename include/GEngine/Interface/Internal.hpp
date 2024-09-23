/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Internal.hpp
*/

#pragma once

#include "GEngine/interface/Base.hpp"
#include "GEngine/game/Engine.hpp"

#include <unistd.h>


namespace gengine::interface {
    class Internal: public Base {
        public:
            Internal(game::Engine &gameEngine, driver::Engine &driverEngine): m_gameEngine(gameEngine), m_driverEngine(driverEngine) {};
            void run(void) override {
                m_gameEngine.start();
                m_driverEngine.start();
                while(1) {
                    m_gameEngine.update();
                    m_driverEngine.update();
                    usleep(100000);
                }
            }
        private:
            game::Engine &m_gameEngine;
            driver::Engine &m_driverEngine;
    };
}
