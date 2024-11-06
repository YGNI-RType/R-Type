/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** ScoreText.hpp
*/

#pragma once

#include "GEngine/libdev/Component.hpp"

namespace rtype::component {
struct ScoreText : public gengine::Component<ScoreText> {
    ScoreText() {
    }

    bool operator==(const ScoreText &) const = default;
};
} // namespace rtype::component
