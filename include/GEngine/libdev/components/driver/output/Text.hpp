/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Text.hpp
*/

#pragma once

#include <raylib.h>
#include <string>

#include "GEngine/libdev/Component.hpp"

namespace gengine::component::driver::output {
struct Text : public gengine::Component {
    std::string fontPath;
    std::string str;
    float fontSize;
    float spacing;
    Color tint;

    Text(const std::string &fontPath, const std::string &str, const Color &tint = BLACK, float spacing = 0)
        : fontPath(fontPath), str(str), fontSize(10), spacing(spacing), tint(tint) {}
};
} // namespace gengine::component::driver::output
