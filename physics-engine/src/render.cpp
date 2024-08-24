#include "render.hpp"

void render(uint32_t* image, uint16_t width, uint16_t height, Color canvasColor, const Entity* entities,
            uint32_t entityCount)
{
    uint32_t canvasPixel = 0xFFFFFFFF
                           & canvasColor.r
                           & (static_cast<uint32_t>(canvasColor.g) << 8)
                           & (static_cast<uint32_t>(canvasColor.b) << 16);

    for (size_t i = 0; i < width * height; ++i)
    {
        image[i] = canvasPixel;
    }
}