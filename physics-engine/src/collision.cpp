#include "collision.hpp"

bool iterateCollisions(std::vector<Entity>& entities)
{
    bool resolvedCollision = false;

    for (int i = 0; i < entities.size(); ++i)
        for (int j = i + 1; j < entities.size(); ++j)
        {
            // resolve potential collision between entities[i] and entities[j]
            if (resolveCollision(entities[i], entities[j]))
                resolvedCollision = true;
        }

    return resolvedCollision;
}

bool resolveCollision(Entity& a, Entity& b)
{
    Vec2 offset = a.position - b.position;
    float sqrDist = offset.sqrMagnitude();

    if (sqrDist > precisionErr && sqrDist < entityRadius * 4)
    {
        // calculate overlapping offset
        float m = std::sqrt(sqrDist);
        float overlap = entityRadius * 2 - m;
        offset.x *= overlap / (2 * m);
        offset.y *= overlap / (2 * m);

        // move entities to resolve overlap
        a.position += offset;
        b.position -= offset;

        return true;
    }

    return false;
}