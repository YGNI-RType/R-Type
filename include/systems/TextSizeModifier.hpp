/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** TextSizeModifier.hpp
*/

#pragma once

#include "GEngine/libdev/Components.hpp"
#include "GEngine/libdev/Events.hpp"
#include "GEngine/libdev/System.hpp"

#include "events/TextSize.hpp"

namespace rtype::system::gui {
class TextSizeModifier : public gengine::System<TextSizeModifier, geg::component::io::Text> {
public:
    void init(void) final;

    void onRenderLoop(geg::event::RenderLoop &);
    void onChangeTextSize(event::ChangeTextSize &);

private:
    int m_textSize = 10;
};
} // namespace rtype::system::gui