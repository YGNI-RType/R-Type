/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** GameInputs.hpp
*/

#pragma once

#include "GEngine/libdev/Entity.hpp"
#include "GEngine/libdev/Events.hpp"
#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/systems/driver/input/KeyboardCatcher.hpp"

namespace rtype::event::in {
enum Input { UP, LEFT, DOWN, RIGHT, SHOOT, CHEAT };
using InputState = geg::event::io::InputState;
struct GameInput : public gengine::Event {
    InputState state;
    GameInput(InputState state)
        : state(state) {
    }
};
struct Up : public GameInput {
    Up(InputState state)
        : GameInput(state) {
    }
};
struct Left : public GameInput {
    Left(InputState state)
        : GameInput(state) {
    }
};
struct Down : public GameInput {
    Down(InputState state)
        : GameInput(state) {
    }
};
struct Right : public GameInput {
    Right(InputState state)
        : GameInput(state) {
    }
};
struct Shoot : public GameInput {
    Shoot(InputState state)
        : GameInput(state) {
    }
};
struct Cheat : public GameInput {
    Cheat(InputState state)
        : GameInput(state) {
    }
};

struct ChangeInputKey : gengine::Event {
    Input input;
    int newKey;
    ChangeInputKey(Input input, int newKey)
        : input(input), newKey(newKey) {
    }
};
} // namespace rtype::event::in
