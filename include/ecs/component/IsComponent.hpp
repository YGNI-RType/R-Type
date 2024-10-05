/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** IsComponent.hpp
*/

#pragma once

#include <concepts>

namespace ecs::component {

// TODO Concept to check if a type has operator==
// template <typename T>
// concept EqualityComparable = requires(const T &a, const T &b) {
//     { a == b } -> std::convertible_to<bool>;
// };

template <typename Derived>
struct IsComponent {
    bool operator==(const IsComponent<Derived> &) const = default;
};
}
