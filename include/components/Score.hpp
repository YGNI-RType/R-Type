/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Score.hpp
*/

#pragma once

#include "GEngine/libdev/Component.hpp"

namespace rtype::component {
struct Score : public gengine::Component<Score> {
    Score(int score)
        : score(score) {
    }
    int score = 0;
};
} // namespace rtype::component
