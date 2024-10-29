/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Types
*/

#pragma once

#include "GEngine/libdev/Components.hpp"

#include <nlohmann/json.hpp>

namespace rtype {
struct Background {
    geg::component::io::Sprite sprite;
    geg::component::Transform2D transform;
    geg::component::Velocity2D velocity;
};

enum TypeOfMotion { FLAPPING, BOUNDING, LINEAR };

struct Mob {
    geg::component::io::Sprite sprite;
    geg::component::io::Animation animation;
    geg::component::Transform2D transform;
    geg::component::Velocity2D velocity;
    TypeOfMotion typeOfMotion;
    bool isShooting;
};

struct Boss {
    geg::component::io::Sprite sprite;
    geg::component::io::Animation animation;
    geg::component::Transform2D transform;
    geg::component::Velocity2D velocity;
    std::vector<std::string> wavesName;
    int waveCooldown;
    int ballSpeed;
    int minVelocity;
    int maxVelocity;
    float borderMargin;
};

struct Monster {
    std::string mobName;
    gengine::Vect2 spawnFactor;
    unsigned int spawnDelay;
    float scaleFactor;
    float speedFactor;
    int scoreGain;
    unsigned int numberOfLifes;
    bool isBoss;
};

struct Stage {
    Background background;
    std::vector<Monster> monsters;
};

void to_json(nlohmann::json &j, const Background &b);
void from_json(const nlohmann::json &j, Background &b);

void to_json(nlohmann::json &j, const Mob &m);
void from_json(const nlohmann::json &j, Mob &m);

void to_json(nlohmann::json &j, const Boss &b);
void from_json(const nlohmann::json &j, Boss &b);

void to_json(nlohmann::json &j, const Monster &m);
void from_json(const nlohmann::json &j, Monster &m);

void to_json(nlohmann::json &j, const std::vector<Monster> &m);
void from_json(const nlohmann::json &j, std::vector<Monster> &m);

void to_json(nlohmann::json &j, const Stage &s);
void from_json(const nlohmann::json &j, Stage &s);
} // namespace rtype

namespace Network {
template <size_t Size>
void to_json(nlohmann::json &j, const NetString<Size> &s) {
    std::string str = s.c_str();
    j = nlohmann::json{str};
}

template <size_t Size>
void from_json(const nlohmann::json &j, NetString<Size> &s) {
    std::string str;
    j.get_to(str);
    s = str;
}
} // namespace Network

namespace gengine {
void to_json(nlohmann::json &j, const Vect2 &v);
void from_json(const nlohmann::json &j, Vect2 &v);
} // namespace gengine

namespace gengine::component {
// TODO implemente default constructor at Transform2D
void to_json(nlohmann::json &j, const Transform2D &t);
void from_json(const nlohmann::json &j, Transform2D &t);

// TODO implemente default constructor at Velocity2D
void to_json(nlohmann::json &j, const Velocity2D &v);
void from_json(const nlohmann::json &j, Velocity2D &v);
} // namespace gengine::component

namespace gengine::component::driver::output {
// TODO implemente default constructor at Rect
void to_json(nlohmann::json &j, const Rect &r);
void from_json(const nlohmann::json &j, Rect &r);

// TODO implemente default constructor at Clr
void to_json(nlohmann::json &j, const Clr &c);
void from_json(const nlohmann::json &j, Clr &c);

// TODO implemente default constructor at Sprite
void to_json(nlohmann::json &j, const Sprite &s);
void from_json(const nlohmann::json &j, Sprite &s);

// TODO implemente default constructor at Animation
void to_json(nlohmann::json &j, const Animation &a);
void from_json(const nlohmann::json &j, Animation &a);
} // namespace gengine::component::driver::output
