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

    virtual const char* BaseException::what() const noexcept override {
        return message_.c_str();
    }
}
