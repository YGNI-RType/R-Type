/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** InputBoxMapper.hpp
*/

#pragma once

#include "GEngine/libdev/Component.hpp"
#include "events/GameInputs.hpp"

namespace rtype::component {
struct InputBoxMapper : public gengine::Component<InputBoxMapper> {
    event::in::Input input;

    InputBoxMapper(event::in::Input input)
        : input(input) {
    }
    bool operator==(const InputBoxMapper &) const = default;
};
} // namespace rtype::component
