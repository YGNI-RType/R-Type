/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** AutoMotion.hpp
*/

#pragma once

#include "libdev/System.hpp"
#include "libdev/systems/events/MainLoop.hpp"

#include "libdev/components/Motions.hpp"

#include "libdev/systems/events/driver/input/Keyboard.hpp"

namespace hagarioop::systems {
class AutoMotion : public gengine::System<AutoMotion, gengine::component::Motion2D> {
public:
    void init(void) override;
    void onMainLoop(gengine::system::event::MainLoop &);
    void onKeyLeft(gengine::system::event::driver::input::Key_Left &);
    void onKeyRight(gengine::system::event::driver::input::Key_Right &);
    void onKeyUp(gengine::system::event::driver::input::Key_Up &);
    void onKeyDown(gengine::system::event::driver::input::Key_Down &);
};
} // namespace hagarioop::systems
