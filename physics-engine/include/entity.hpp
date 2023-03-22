#pragma once

#include <cstdint>
#include "math.hpp"
#include "constraint.hpp"

struct Color
{
    uint8_t r;
    uint8_t g;
    uint8_t b;

    Color() : r{255}, g{255}, b{255}
    {}

    Color(uint8_t r, uint8_t g, uint8_t b) : r{r}, g{g}, b{b}
    {}
};

constexpr float entityRadius = 1.0;

struct Entity
{
    Vec2 position;
    Vec2 lastPosition;
    Vec2 acceleration;
    Color color;

    Entity() = default;

    Entity(float x, float y) : position{x, y}, lastPosition{x, y}, acceleration{}, color{}
    {}

    Entity(Vec2 position, Color color) : position{position}, lastPosition{position}, acceleration{}, color{color}
    {}

    void update(float dt)
    {
        Vec2 velocity = position - lastPosition;
        lastPosition = position;
        position += velocity + acceleration * (dt * dt);
        acceleration = {};
    }

    void accelerate(Vec2 force)
    {
        acceleration += force;
    }

    void constrain(const BoxConstraint& constraint)
    {
        if (position.x < constraint.left)
            position.x = constraint.left;
        else if (position.x > constraint.right)
            position.x = constraint.right;

        if (position.y < constraint.bottom)
            position.y = constraint.bottom;
        else if (position.y > constraint.top)
            position.y = constraint.top;
    }
};