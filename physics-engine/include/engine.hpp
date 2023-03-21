#pragma once

#include <vector>
#include <cstdint>
#include "entity.hpp"

enum class PourStyle {
    center
};

const uint16_t targetFps = 60;
const uint8_t numThreads = 1;
const PourStyle style = PourStyle::center;

void liquify(const std::vector<Color>& pixels, uint16_t width, uint16_t height);