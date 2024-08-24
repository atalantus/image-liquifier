#pragma once

#include <cstdint>
#include "entity.hpp"
#include "constraint.hpp"
#include "collision.hpp"

struct Engine
{
    /// sub step count
    uint16_t subSteps = 0;

    /// total simulation time
    float time = 0.0f;
    /// update delta time
    float updateDt = 0.0f;
    /// step delta time
    float stepDt = 0.0f;

    /// gravity
    Vec2 gravity = {0.0f, 1000.0f};

    /// entities
    Entity* entities = nullptr;
    /// world constraint
    BoxConstraint constraint{0, 0, 0, 0};

    uint32_t entityCount = 0;

    Engine& setUpdateRate(uint16_t updateRate, uint16_t subStepCount)
    {
        subSteps = subStepCount;
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
        entities[entityCount++] = {position, color};
        return *this;
    }

    Engine& instantiateEntity(Vec2 position, Vec2 initialVelocity, Color color)
    {
        entities[entityCount++] = {position, color, initialVelocity, stepDt};
        return *this;
    }

    void update();
};