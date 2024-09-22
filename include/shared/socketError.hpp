/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet (Workspace)
** File description:
** socketError
*/

#pragma once

#include <exception>

namespace Network {

class SocketException : public std::exception {
  public:
    SocketException(const char *msg) : m_msg(msg) {}

    const char *what() const noexcept override {
        return m_msg;
    }
  private:
    const char *m_msg;
};
} // namespace Network
