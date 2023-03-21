#pragma once

#include "math.hpp"
#include <cstdint>

struct Color
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

constexpr float entityRadius = 1.0;

struct Entity
{
    Vec2 position;
    Vec2 velocity;
    Color color;

    Entity() : position{0, 0}, velocity{0, 0}, color{0, 0, 0}
    {}

    Entity(float x, float y) : position{x, y}, velocity{0, 0}, color{0, 0, 0}
    {}
};