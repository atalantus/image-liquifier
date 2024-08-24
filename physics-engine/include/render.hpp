#pragma once

#include "entity.hpp"

void render(uint32_t* image, uint16_t width, uint16_t height, Color canvasColor, const Entity* entities, uint32_t entityCount);