/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** TextSize.hpp
*/

#pragma once

#include "GEngine/libdev/Entity.hpp"
#include "GEngine/libdev/System.hpp"

namespace rtype::event {
struct ChangeTextSize : public gengine::Event {
    int diff;
    ChangeTextSize(int diff)
        : diff(diff) {
    }
};
} // namespace rtype::event
