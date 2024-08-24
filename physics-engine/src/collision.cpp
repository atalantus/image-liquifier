#include "collision.hpp"

void resolveCollision(Entity& a, Entity& b)
{
    Vec2 offset = a.position - b.position;
    float sqrDist = offset.sqrMagnitude();

    if (sqrDist > precisionErr && sqrDist < entityRadius * 4)
    {
        // calculate overlapping offset
        float m = sqrtf(sqrDist);
        float overlap = entityRadius * 2 - m;
        offset.x *= overlap / (2 * m);
        offset.y *= overlap / (2 * m);

        // move entities to resolve overlap
        a.position += offset;
        b.position -= offset;
    }
}