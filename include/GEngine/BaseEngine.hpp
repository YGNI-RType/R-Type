/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** AEngine.hpp
*/

#pragma once

#include "GEngine/Interface/Base.hpp"
#include <memory>

namespace gengine {
    class BaseEngine {
    public:
        BaseEngine(std::unique_ptr<interface::Base> interface): m_interface(std::move(interface)) {};

        void run(void);

    private:
            std::unique_ptr<interface::Base> m_interface;
    };
}
