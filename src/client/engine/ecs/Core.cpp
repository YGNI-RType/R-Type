/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Core.cpp
*/

#include "Core.hpp"
#include <iostream>

using namespace ecs;

Core::Core()
{
}

void Core::killEntity(Entity entity)
{
    destroyComponents(entity);
    destroyEntity(entity);
}
