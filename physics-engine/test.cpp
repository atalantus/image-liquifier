#include <iostream>
#include <cmath>
#include <vector>
#include <cassert>

#include "engine.hpp"
#include "collision.hpp"

double err = 1e-12;
uint32_t maxSteps = 100;

bool DBLEQL(double a, double b)
{
    return std::abs(a - b) <= err;
}

bool operator==(Vec2 a, Vec2 b)
{
    return DBLEQL(a.x, b.x) && DBLEQL(a.y, b.y);
}

int main()
{
    std::vector<Entity> entities1 = {{0,  0},
                                     {1,  0},
                                     {2,  0},
                                     {99, 99}};
    Entity& e1 = entities1[0];
    Entity& e2 = entities1[1];
    Entity& e3 = entities1[2];
    Entity& e4 = entities1[3];

    assert(DBLEQL((e1.position - e2.position).magnitude(), 1));
    assert(DBLEQL((e2.position - e3.position).magnitude(), 1));
    assert(DBLEQL((e1.position - e3.position).magnitude(), 2));

    resolveCollision(e1, e3);
    assert((e1.position == Vec2{0, 0}));
    assert((e2.position == Vec2{1, 0}));
    assert((e3.position == Vec2{2, 0}));
    assert((e4.position == Vec2{99, 99}));

    resolveCollision(e1, e2);
    assert((e1.position == Vec2{-0.5, 0}));
    assert((e2.position == Vec2{1.5, 0}));
    assert((e3.position == Vec2{2, 0}));
    assert((e4.position == Vec2{99, 99}));

    resolveCollision(e1, e2);
    assert((e1.position == Vec2{-0.5, 0}));
    assert((e2.position == Vec2{1.5, 0}));
    assert((e3.position == Vec2{2, 0}));
    assert((e4.position == Vec2{99, 99}));

    resolveCollision(e2, e3);
    assert((e1.position == Vec2{-0.5, 0}));
    assert((e2.position == Vec2{0.75, 0}));
    assert((e3.position == Vec2{2.75, 0}));
    assert((e4.position == Vec2{99, 99}));

    for (int i = 0; i < maxSteps; ++i)
        for (int j = 0; j < entities1.size(); ++j)
            for (int k = j + 1; k < entities1.size(); ++k)
                resolveCollision(entities1[j], entities1[k]);

    assert((e1.position == Vec2{-1, 0}));
    assert((e2.position == Vec2{1, 0}));
    assert((e3.position == Vec2{3, 0}));
    assert((e4.position == Vec2{99, 99}));

    std::vector<Entity> entities2{{0,  0},
                                  {1,  0},
                                  {1,  1},
                                  {0,  1},
                                  {-1, 1},
                                  {-1, 0},
                                  {-1, -1},
                                  {0,  -1},
                                  {1,  -1}
    };

    for (int i = 0; i < maxSteps; ++i)
        for (int j = 0; j < entities2.size(); ++j)
            for (int k = j + 1; k < entities2.size(); ++k)
                resolveCollision(entities2[j], entities2[k]);

    for (int j = 0; j < entities2.size(); ++j)
        for (int k = j + 1; k < entities2.size(); ++k)
        {
            float dist = (entities2[j].position - entities2[k].position).magnitude();
            assert((dist >= 1.0f));
        }
}