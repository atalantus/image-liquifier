#include <iostream>
#include <cmath>
#include <vector>
#include <cassert>

#include "engine.hpp"
#include "collision.hpp"

double err = 1e-12;

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

    assert(!resolveCollision(e1, e3));
    assert((e1.position == Vec2{0, 0}));
    assert((e2.position == Vec2{1, 0}));
    assert((e3.position == Vec2{2, 0}));
    assert((e4.position == Vec2{99, 99}));

    assert(resolveCollision(e1, e2));
    assert((e1.position == Vec2{-0.5, 0}));
    assert((e2.position == Vec2{1.5, 0}));
    assert((e3.position == Vec2{2, 0}));
    assert((e4.position == Vec2{99, 99}));

    assert(!resolveCollision(e1, e2));
    assert((e1.position == Vec2{-0.5, 0}));
    assert((e2.position == Vec2{1.5, 0}));
    assert((e3.position == Vec2{2, 0}));
    assert((e4.position == Vec2{99, 99}));

    assert(resolveCollision(e2, e3));
    assert((e1.position == Vec2{-0.5, 0}));
    assert((e2.position == Vec2{0.75, 0}));
    assert((e3.position == Vec2{2.75, 0}));
    assert((e4.position == Vec2{99, 99}));

    uint64_t steps = 0;
    while (iterateCollisions(entities1)) steps++;
    std::cout << steps << std::endl;

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

    steps = 0;
    while (iterateCollisions(entities2)) steps++;
    std::cout << steps << std::endl;

    assert(!iterateCollisions(entities2));
}