/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** GameState.hpp
*/

#pragma once

#include "GEngine/libdev/Component.hpp"

namespace rtype::component {
struct GameState : public gengine::Component<GameState> {
    enum State { LOBBY, GAME, GAMEOVER, WIN };
    State state;
    GameState(State state)
        : state(state) {
    }

    bool operator==(const GameState &) const = default;
};
} // namespace rtype::component
