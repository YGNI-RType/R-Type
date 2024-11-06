/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** NextStage.hpp
*/

#pragma once

#include "GEngine/libdev/System.hpp"

namespace rtype::event {
struct NextStage : public gengine::Event {
    NextStage() {
    }
};

struct GoToLobby : public gengine::Event {
    GoToLobby() {
    }
};
} // namespace rtype::event
