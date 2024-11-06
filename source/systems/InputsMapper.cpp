/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** InputsMapper.cpp
*/

#include "systems/InputsMapper.hpp"

namespace rtype::system {
void InputsMapper::init(void) {
    subscribeToEvent<geg::event::io::KeyPressedEvent>(&InputsMapper::handleKeyPressed);
    subscribeToEvent<geg::event::io::KeyReleasedEvent>(&InputsMapper::handleKeyReleased);
    subscribeToEvent<event::in::ChangeInputKey>(&InputsMapper::changeInputKey);
}

void InputsMapper::handleKeyPressed(geg::event::io::KeyPressedEvent &e) {
    if (e.key == m_upKey)
        publishEvent(event::in::Up(event::in::InputState::PRESSED));
    if (e.key == m_leftKey)
        publishEvent(event::in::Left(event::in::InputState::PRESSED));
    if (e.key == m_downKey)
        publishEvent(event::in::Down(event::in::InputState::PRESSED));
    if (e.key == m_rightKey)
        publishEvent(event::in::Right(event::in::InputState::PRESSED));
    if (e.key == m_shootKey)
        publishEvent(event::in::Shoot(event::in::InputState::PRESSED));
    if (e.key == m_cheatKey)
        publishEvent(event::in::Cheat(event::in::InputState::PRESSED));
    if (e.key == m_voiceKey)
        publishEvent(event::in::VoiceChat(event::in::InputState::PRESSED));
    if (e.key == m_recordKey)
        publishEvent(event::in::Record(event::in::InputState::PRESSED));
}

void InputsMapper::handleKeyReleased(geg::event::io::KeyReleasedEvent &e) {
    if (e.key == m_upKey)
        publishEvent(event::in::Up(event::in::InputState::RELEASE));
    if (e.key == m_leftKey)
        publishEvent(event::in::Left(event::in::InputState::RELEASE));
    if (e.key == m_downKey)
        publishEvent(event::in::Down(event::in::InputState::RELEASE));
    if (e.key == m_rightKey)
        publishEvent(event::in::Right(event::in::InputState::RELEASE));
    if (e.key == m_shootKey)
        publishEvent(event::in::Shoot(event::in::InputState::RELEASE));
    if (e.key == m_cheatKey)
        publishEvent(event::in::Cheat(event::in::InputState::RELEASE));
    if (e.key == m_voiceKey)
        publishEvent(event::in::VoiceChat(event::in::InputState::RELEASE));
    if (e.key == m_recordKey)
        publishEvent(event::in::Record(event::in::InputState::RELEASE));
}

void InputsMapper::changeInputKey(event::in::ChangeInputKey &e) {
    switch (e.input) {
    case event::in::Input::UP:
        m_upKey = e.newKey;
        break;
    case event::in::Input::LEFT:
        m_leftKey = e.newKey;
        break;
    case event::in::Input::DOWN:
        m_downKey = e.newKey;
        break;
    case event::in::Input::RIGHT:
        m_rightKey = e.newKey;
        break;
    case event::in::Input::SHOOT:
        m_shootKey = e.newKey;
        break;
    case event::in::Input::CHEAT:
        m_cheatKey = e.newKey;
        break;
    case event::in::Input::VOICE:
        m_voiceKey = e.newKey;
        break;
    case event::in::Input::RECORD:
        m_recordKey = e.newKey;
        break;
    default:
        break;
    }
}

void InputBoxMapperHandler::onEvent(geg::event::io::KeyPressedEvent &e) {
    auto &inputs = getComponents<component::InputBoxMapper>();
    auto &buttons = getComponents<gengine::component::gui::SelectButton>();
    auto &boxes = getComponents<geg::component::Box<1>>();
    auto &texts = getComponents<geg::component::io::Text>();

    for (auto [entity, button, input, box] : gengine::Zip(buttons, inputs, boxes)) {
        if (button.state == button.PRESSED) {
            button.state = button.RELEASED;
            if (texts.contains(box.entities[0])) {
                auto it = keyToString.find(e.key);
                texts.get(box.entities[0]).str = it->second;
            }
            publishEvent(event::in::ChangeInputKey(input.input, e.key));
            return;
        }
    }
}
} // namespace rtype::system
