/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** InputsToGameEvents.cpp
*/

#include "systems/InputsToGameEvents.hpp"

#include "GEngine/interface/network/events/Record.hpp"
#include "GEngine/libdev/systems/events/driver/input/VoIP.hpp"

namespace rtype::system {
void InputsToGameEvents::init(void) {
    subscribeToEvent<gengine::system::event::GameLoop>(&InputsToGameEvents::sendEvents);
    subscribeToEvent<event::in::Up>(&InputsToGameEvents::moveUp);
    subscribeToEvent<event::in::Left>(&InputsToGameEvents::moveLeft);
    subscribeToEvent<event::in::Down>(&InputsToGameEvents::moveDown);
    subscribeToEvent<event::in::Right>(&InputsToGameEvents::moveRight);
    subscribeToEvent<event::in::Shoot>(&InputsToGameEvents::shoot);
    subscribeToEvent<event::in::Cheat>(&InputsToGameEvents::becomeInvincible);
    subscribeToEvent<event::in::VoiceChat>(&InputsToGameEvents::voiceChat);
    subscribeToEvent<event::in::Record>(&InputsToGameEvents::record);
}

void InputsToGameEvents::sendEvents(gengine::system::event::GameLoop &e) {
    publishEvent<event::Movement>(getMovementState());
    publishEvent<event::Shoot>(m_shootState);
}

void InputsToGameEvents::moveUp(event::in::Up &e) {
    switch (e.state) {
    case geg::event::io::InputState::PRESSED:
        updateDirectionBitmask(event::Movement::UP, true);
        break;
    case geg::event::io::InputState::RELEASE:
        updateDirectionBitmask(event::Movement::UP, false);
        break;
    default:
        break;
    }
}
void InputsToGameEvents::moveLeft(event::in::Left &e) {
    switch (e.state) {
    case geg::event::io::InputState::PRESSED:
        updateDirectionBitmask(event::Movement::LEFT, true);
        break;
    case geg::event::io::InputState::RELEASE:
        updateDirectionBitmask(event::Movement::LEFT, false);
        break;
    default:
        break;
    }
}
void InputsToGameEvents::moveDown(event::in::Down &e) {
    switch (e.state) {
    case geg::event::io::InputState::PRESSED:
        updateDirectionBitmask(event::Movement::DOWN, true);
        break;
    case geg::event::io::InputState::RELEASE:
        updateDirectionBitmask(event::Movement::DOWN, false);
        break;
    default:
        break;
    }
}
void InputsToGameEvents::moveRight(event::in::Right &e) {
    switch (e.state) {
    case geg::event::io::InputState::PRESSED:
        updateDirectionBitmask(event::Movement::RIGHT, true);
        break;
    case geg::event::io::InputState::RELEASE:
        updateDirectionBitmask(event::Movement::RIGHT, false);
        break;
    default:
        break;
    }
}

void InputsToGameEvents::shoot(event::in::Shoot &e) {
    switch (e.state) {
    case geg::event::io::InputState::PRESSED:
        m_shootState = event::Shoot::CHARGING;
        break;
    case geg::event::io::InputState::RELEASE:
        m_shootState = event::Shoot::REST;
        break;
    default:
        break;
    }
}

void InputsToGameEvents::updateDirectionBitmask(event::Movement::State state, bool pressed) {
    char mask = 0;

    switch (state) {
    case event::Movement::UP:
        mask = UP_MASK;
        break;
    case event::Movement::RIGHT:
        mask = RIGHT_MASK;
        break;
    case event::Movement::DOWN:
        mask = DOWN_MASK;
        break;
    case event::Movement::LEFT:
        mask = LEFT_MASK;
        break;
    default:
        return;
    }

    if (pressed)
        m_directionBitmask |= mask;
    else
        m_directionBitmask &= ~mask;
}

event::Movement::State InputsToGameEvents::getMovementState(void) {
    switch (m_directionBitmask) {
    case UP_MASK:
        return event::Movement::UP;
    case DOWN_MASK:
        return event::Movement::DOWN;
    case LEFT_MASK:
        return event::Movement::LEFT;
    case RIGHT_MASK:
        return event::Movement::RIGHT;
    case UP_MASK | LEFT_MASK:
        return event::Movement::UP_LEFT;
    case UP_MASK | RIGHT_MASK:
        return event::Movement::UP_RIGHT;
    case DOWN_MASK | LEFT_MASK:
        return event::Movement::DOWN_LEFT;
    case DOWN_MASK | RIGHT_MASK:
        return event::Movement::DOWN_RIGHT;
    default:
        return event::Movement::STANDING;
    }
}

void InputsToGameEvents::becomeInvincible(event::in::Cheat &e) {
    switch (e.state) {
    case geg::event::io::InputState::PRESSED:
        publishEvent(event::BecomeInvincible(true));
        break;
    case geg::event::io::InputState::RELEASE:
        publishEvent(event::BecomeInvincible(false));
        break;
    default:
        break;
    }
}

void InputsToGameEvents::voiceChat(event::in::VoiceChat &e) {
    switch (e.state) {
    case geg::event::io::InputState::PRESSED:
        publishEvent(gengine::system::event::driver::input::StartVoIP());
        break;
    case geg::event::io::InputState::RELEASE:
        publishEvent(gengine::system::event::driver::input::EndVoIP());
        break;
    default:
        break;
    }
}
void InputsToGameEvents::record(event::in::Record &e) {
    if (e.state == geg::event::io::InputState::PRESSED)
        publishEvent(gengine::interface::network::event::ToogleRecord());
}
} // namespace rtype::system
