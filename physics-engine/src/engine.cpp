#include "engine.hpp"

void Engine::update()
{
    time += updateDt;

    for (uint16_t i = 0; i < subSteps; ++i)
    {
        for (size_t j = 0; j < entities.size(); ++j)
        {
            Entity& entity = entities[j];
            // apply gravity
            entity.accelerate(gravity);
            // apply collisions
            for (size_t k = j + 1; k < entities.size(); ++k)
                resolveCollision(entity, entities[k]);
            // apply constraint
            entity.constrain(constraint);
            // update entity
            entity.update(stepDt);
        }
    }
}
