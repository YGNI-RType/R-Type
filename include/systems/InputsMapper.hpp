/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** InputsMapper.hpp
*/

#pragma once

#include "GEngine/libdev/Events.hpp"
#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/Systems.hpp"
#include "GEngine/libdev/Components.hpp"

#include "events/GameInputs.hpp"
#include "components/InputBoxMapper.hpp"

namespace rtype::system {
class InputsMapper : public gengine::System<InputsMapper>, public gengine::LocalSystem {
public:
    void init(void) override;

    void handleKeyPressed(geg::event::io::KeyPressedEvent &);
    void handleKeyReleased(geg::event::io::KeyReleasedEvent &);

    void changeInputKey(event::in::ChangeInputKey &e);

private:
    int m_upKey = KEY_UP;
    int m_leftKey = KEY_LEFT;
    int m_downKey = KEY_DOWN;
    int m_rightKey = KEY_RIGHT;
    int m_shootKey = KEY_SPACE;
    int m_cheatKey = KEY_LEFT_SHIFT;
};

class InputBoxMapperHandler : public gengine::OnEventSystem<InputBoxMapperHandler, geg::event::io::KeyPressedEvent, geg::component::gui::SelectButton, component::InputBoxMapper,geg::component::io::Text, geg::component::Box<1>> {
    public:
        void onEvent(geg::event::io::KeyPressedEvent &) final;
};

static const std::unordered_map<int, std::string> keyToString = {
    {KEY_NULL, "NULL"},
    {KEY_APOSTROPHE, "'"},
    {KEY_COMMA, ","},
    {KEY_MINUS, "-"},
    {KEY_PERIOD, "."},
    {KEY_SLASH, "/"},
    {KEY_ZERO, "0"},
    {KEY_ONE, "1"},
    {KEY_TWO, "2"},
    {KEY_THREE, "3"},
    {KEY_FOUR, "4"},
    {KEY_FIVE, "5"},
    {KEY_SIX, "6"},
    {KEY_SEVEN, "7"},
    {KEY_EIGHT, "8"},
    {KEY_NINE, "9"},
    {KEY_SEMICOLON, ";"},
    {KEY_EQUAL, "="},
    {KEY_A, "A"},
    {KEY_B, "B"},
    {KEY_C, "C"},
    {KEY_D, "D"},
    {KEY_E, "E"},
    {KEY_F, "F"},
    {KEY_G, "G"},
    {KEY_H, "H"},
    {KEY_I, "I"},
    {KEY_J, "J"},
    {KEY_K, "K"},
    {KEY_L, "L"},
    {KEY_M, "M"},
    {KEY_N, "N"},
    {KEY_O, "O"},
    {KEY_P, "P"},
    {KEY_Q, "Q"},
    {KEY_R, "R"},
    {KEY_S, "S"},
    {KEY_T, "T"},
    {KEY_U, "U"},
    {KEY_V, "V"},
    {KEY_W, "W"},
    {KEY_X, "X"},
    {KEY_Y, "Y"},
    {KEY_Z, "Z"},
    {KEY_LEFT_BRACKET, "["},
    {KEY_BACKSLASH, "\\"},
    {KEY_RIGHT_BRACKET, "]"},
    {KEY_GRAVE, "`"},
    {KEY_SPACE, "SPACE"},
    {KEY_ESCAPE, "ESC"},
    {KEY_ENTER, "ENTER"},
    {KEY_TAB, "TAB"},
    {KEY_BACKSPACE, "BACKSPACE"},
    {KEY_INSERT, "INS"},
    {KEY_DELETE, "DEL"},
    {KEY_RIGHT, "RIGHT"},
    {KEY_LEFT, "LEFT"},
    {KEY_DOWN, "DOWN"},
    {KEY_UP, "UP"},
    {KEY_PAGE_UP, "PAGE_UP"},
    {KEY_PAGE_DOWN, "PAGE_DOWN"},
    {KEY_HOME, "HOME"},
    {KEY_END, "END"},
    {KEY_CAPS_LOCK, "CAPS_LOCK"},
    {KEY_SCROLL_LOCK, "SCROLL_LOCK"},
    {KEY_NUM_LOCK, "NUM_LOCK"},
    {KEY_PRINT_SCREEN, "PRINT_SCREEN"},
    {KEY_PAUSE, "PAUSE"},
    {KEY_F1, "F1"},
    {KEY_F2, "F2"},
    {KEY_F3, "F3"},
    {KEY_F4, "F4"},
    {KEY_F5, "F5"},
    {KEY_F6, "F6"},
    {KEY_F7, "F7"},
    {KEY_F8, "F8"},
    {KEY_F9, "F9"},
    {KEY_F10, "F10"},
    {KEY_F11, "F11"},
    {KEY_F12, "F12"},
    {KEY_LEFT_SHIFT, "LSHIFT"},
    {KEY_LEFT_CONTROL, "LCTRL"},
    {KEY_LEFT_ALT, "LALT"},
    {KEY_LEFT_SUPER, "LSUPER"},
    {KEY_RIGHT_SHIFT, "RSHIFT"},
    {KEY_RIGHT_CONTROL, "RCTRL"},
    {KEY_RIGHT_ALT, "RALT"},
    {KEY_RIGHT_SUPER, "RSUPER"},
    {KEY_KB_MENU, "KB_MENU"},
    {KEY_KP_0, "0"},
    {KEY_KP_1, "1"},
    {KEY_KP_2, "2"},
    {KEY_KP_3, "3"},
    {KEY_KP_4, "4"},
    {KEY_KP_5, "5"},
    {KEY_KP_6, "6"},
    {KEY_KP_7, "7"},
    {KEY_KP_8, "8"},
    {KEY_KP_9, "9"},
    {KEY_KP_DECIMAL, "."},
    {KEY_KP_DIVIDE, "/"},
    {KEY_KP_MULTIPLY, "*"},
    {KEY_KP_SUBTRACT, "-"},
    {KEY_KP_ADD, "+"},
    {KEY_KP_ENTER, "ENTER"},
    {KEY_KP_EQUAL, "="}
};
} // namespace rtype::system
