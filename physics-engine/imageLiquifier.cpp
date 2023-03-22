#include "engine.hpp"
#include "constraint.hpp"

void liquify(const std::vector<Color>& pixels, uint16_t width, uint16_t height)
{
    Engine engine;
    engine.setUpdateRate(60)
            .setSubStepCount(8)
            .setWorldConstraint({static_cast<float>(width), 0.0, static_cast<float>(height), 0.0});

    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
        {
            const Color& pixel = pixels[y * width + x];
            engine.instantiateEntity({static_cast<float>(x), static_cast<float>(y)}, pixel);
        }

    for (int i = 0; i < 1000; ++i)
    {
        engine.update();
    }
}