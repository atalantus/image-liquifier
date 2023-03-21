#pragma once

#include <vector>
#include "entity.hpp"

constexpr float precisionErr = 1e-6;

bool iterateCollisions(std::vector<Entity>& entities);

bool resolveCollision(Entity& a, Entity& b);