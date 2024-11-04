/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** GameOver.hpp
*/

#pragma once

#include "GEngine/libdev/Entity.hpp"
#include "GEngine/libdev/System.hpp"

namespace rtype::event {
struct GameOver : public gengine::Event {
    bool win;
    GameOver(bool win)
        : win(win) {
    }
};
} // namespace rtype::event
