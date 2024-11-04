/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** TextSizeModifier.cpp
*/

#include "systems/TextSizeModifier.hpp"

namespace rtype::system::gui {
void TextSizeModifier::init(void) {
    subscribeToEvent<geg::event::RenderLoop>(&TextSizeModifier::onRenderLoop);
    subscribeToEvent<event::ChangeTextSize>(&TextSizeModifier::onChangeTextSize);
}

void TextSizeModifier::onRenderLoop(geg::event::RenderLoop &) {
    auto &texts = getComponents<geg::component::io::Text>();

    for (auto &[entity, text] : texts)
        text.fontSize = m_textSize;
}

void TextSizeModifier::onChangeTextSize(event::ChangeTextSize &e) {
    m_textSize += e.diff;
}
} // namespace rtype::system::gui