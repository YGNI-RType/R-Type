/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Base.hpp
*/

#pragma once

#include <exception>
#include <string>

namespace gengine {
class BaseException : public std::exception {
public:
    BaseException(std::string &message);
    virtual const char *what() const noexcept override;

private:
    std::string m_message;
};
} // namespace gengine
