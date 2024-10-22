/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** InputsToGameEvents.cpp
*/

#include "systems/InputsToGameEvents.hpp"

namespace rtype::system {
void InputsToGameEvents::init(void) {
    subscribeToEvent<gengine::system::event::GameLoop>(&InputsToGameEvents::sendEvents);
    subscribeToEvent<geg::event::io::KeyUpEvent>(&InputsToGameEvents::moveUp);
    subscribeToEvent<geg::event::io::KeyLeftEvent>(&InputsToGameEvents::moveLeft);
    subscribeToEvent<geg::event::io::KeyDownEvent>(&InputsToGameEvents::moveDown);
    subscribeToEvent<geg::event::io::KeyRightEvent>(&InputsToGameEvents::moveRight);
    subscribeToEvent<geg::event::io::KeySpaceEvent>(&InputsToGameEvents::shoot);
}

void InputsToGameEvents::sendEvents(gengine::system::event::GameLoop &e) {
    publishEvent<event::Movement>(getMovementState());
    publishEvent<event::Shoot>(m_shootState);
}

void InputsToGameEvents::moveUp(geg::event::io::KeyUpEvent &e) {
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
void InputsToGameEvents::moveLeft(geg::event::io::KeyLeftEvent &e) {
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
void InputsToGameEvents::moveDown(geg::event::io::KeyDownEvent &e) {
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
void InputsToGameEvents::moveRight(geg::event::io::KeyRightEvent &e) {
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

void InputsToGameEvents::shoot(geg::event::io::KeySpaceEvent &e) {
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
} // namespace rtype::system
