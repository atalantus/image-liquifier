#pragma once

#include <vector>
#include <cstdint>
#include "entity.hpp"
#include "constraint.hpp"
#include "collision.hpp"

struct Engine
{
    /// sub step count
    uint16_t subSteps = 1;

    /// total simulation time
    float time = 0.0f;
    /// update delta time
    float updateDt = 0.0f;
    /// step delta time
    float stepDt = 0.0f;

    /// gravity
    Vec2 gravity = {0.0f, -10.0f};

    /// entities
    std::vector<Entity> entities;
    /// world constraint
    BoxConstraint constraint{0, 0, 0, 0};

    Engine& setSubStepCount(uint16_t subStepCount)
    {
        subSteps = subStepCount;
        return *this;
    }

    Engine& setUpdateRate(uint16_t updateRate)
    {
        updateDt = 1.0f / static_cast<float>(updateRate);
        stepDt = updateDt / static_cast<float>(subSteps);
        return *this;
    }

    Engine& setWorldConstraint(const BoxConstraint& worldConstraint)
    {
        constraint = worldConstraint;
        return *this;
    }

    Engine& instantiateEntity(Vec2 position, Color color)
    {
        entities.emplace_back(position, color);
        return *this;
    }

    void update();
};