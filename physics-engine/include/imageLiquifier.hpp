#pragma once

#include <cstdint>
#include "engine.hpp"

struct ImageLiquifier
{
    uint32_t width, height;
    uint32_t* pixels;
    Engine engine;

    ImageLiquifier(uint16_t width, uint16_t height, uint32_t* pixels, void* entities) :
            width{width}, height{height}, pixels{pixels}, engine{}
    {
        engine.entities = reinterpret_cast<Entity*>(entities);
    }

    uint32_t* render();

    ImageLiquifier(const ImageLiquifier&) = delete;
    ImageLiquifier(ImageLiquifier&&) = delete;

    ImageLiquifier& operator=(const ImageLiquifier&) = delete;
    ImageLiquifier& operator=(ImageLiquifier&&) = delete;
};