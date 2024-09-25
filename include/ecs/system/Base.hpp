/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Base.hpp
*/

#pragma once

#include <functional>
#include <optional>

#include "ecs/ECS.hpp"
#include "ecs/system/event/Bus.hpp"

namespace ecs::system {
template <typename T, typename... Types> struct is_one_of;

template <typename T, typename First, typename... Rest>
struct is_one_of<T, First, Rest...>
    : std::conditional_t<std::is_same_v<T, First>, std::true_type, is_one_of<T, Rest...>> {};

template <typename T> struct is_one_of<T> : std::false_type {};

class Manager;
template <class Derived, class... DependTypes> class Base {
public:
    Base() = default;

    virtual ~Base() = default;

    virtual void init(void) = 0;

    template <typename EventType> void subscribeToEvent(void (Derived::*callbackMethod)(EventType &));

    template <typename T> T &getSystem(void);

    template <typename T> component::SparseArray<T> &getComponent(void);

    template <typename... Components> void spawnEntity(Components &&...components);

private:
    friend class system::Manager;

    std::optional<std::reference_wrapper<ECS>> m_ecs;
    std::optional<std::reference_wrapper<event::Bus>> m_eventBus;
};
} // namespace ecs::system

#include "Base.inl"
