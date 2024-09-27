/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Draw.hpp
*/

#pragma once

#include <raylib.h>

#include "GEngine/libdev/components/driver/output/Color.hpp"
#include "GEngine/libdev/components/driver/output/Shape.hpp"

#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/systems/events/MainLoop.hpp"
#include "GEngine/libdev/systems/events/Native.hpp"
#include "GEngine/libdev/systems/events/driver/output/Draw.hpp"

#include "GEngine/libdev/components/Positions.hpp"

namespace gengine::system::driver::output {
class Draw : public gengine::System<Draw, component::driver::output::Rectangle, component::driver::output::Color,
                                    component::Position2D> {
public:
    void init(void) override;

    void onDraw(gengine::system::event::Draw &e);

private:
};
} // namespace gengine::system::driver::output