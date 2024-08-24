#pragma once

#include <cstdint>
#include "math.hpp"
#include "constraint.hpp"

struct Color
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    Color() : r{255}, g{255}, b{255}, a{255}
    {}

    Color(uint8_t r, uint8_t g, uint8_t b) : r{r}, g{g}, b{b}, a{255}
    {}
};

constexpr float entityRadius = 1.0;

struct Entity
{
    Vec2 position{};
    Vec2 lastPosition{};
    Vec2 acceleration{};
    Color color;

    Entity() = default;

    Entity(float x, float y) : position{x, y}, lastPosition{x, y}
    {}

    Entity(Vec2 position, Color color) : position{position}, lastPosition{position}, color{color}
    {}

    Entity(Vec2 position, Color color, Vec2 initialVelocity, float dt) :
            position{position}, lastPosition{position - initialVelocity * dt}, color{color}
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

static_assert(sizeof(Entity) == 28);
constexpr size_t EntitySize = sizeof(Entity);