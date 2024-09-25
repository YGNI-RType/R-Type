/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** RenderWindow.hpp
*/

#pragma once

#include <raylib.h>

#include "GEngine/libdev/components/driver/output/Window.hpp"

#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/systems/events/MainLoop.hpp"
#include "GEngine/libdev/systems/events/Native.hpp"
#include "GEngine/libdev/systems/events/driver/output/Draw.hpp"

namespace gengine::system::driver::output {
class RenderWindow : public gengine::System<RenderWindow, gengine::component::driver::output::Window> {
public:
    void init(void) override;

    void onStartEngine(gengine::system::event::StartEngine &e);

    void onMainLoop(gengine::system::event::MainLoop &e);
};
} // namespace gengine::system::driver::output