#pragma once

#include <cmath>
#include "entity.hpp"

constexpr float precisionErr = 1e-6;

void resolveCollision(Entity& a, Entity& b);