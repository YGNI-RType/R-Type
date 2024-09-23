/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Base.cpp
*/

#include "exceptions/Base.hpp"

namespace gengine {
    BaseException::BaseException(std::string &message): m_message(message) {

    }

    const char* BaseException::what() const noexcept {
        return m_message.c_str();
    }
}
