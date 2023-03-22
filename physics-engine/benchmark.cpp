#include <iostream>
#include <chrono>
#include <vector>
#include <numeric>
#include <SFML/Graphics.hpp>

#include "engine.hpp"

#define HEADLESS 0

uint64_t maxEntities = 1000;
uint16_t subStepCount = 8;

uint32_t benchmarkTimeInSeconds = 60;

std::vector<Entity> entities;
BoxConstraint constraint = {static_cast<float>(maxEntities) * 0.5f, 0.0f, static_cast<float>(maxEntities) * 0.6f, 0.0f};
Vec2 instantiationPos = {constraint.right * 0.8f, constraint.top * 0.8f};
Vec2 gravity = {0.0f, -10.0f};

int main()
{

#if !HEADLESS
    sf::ContextSettings settings;
    settings.antialiasingLevel = 1;
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Benchmark", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(false);
    window.setFramerateLimit(60);
#endif

    entities.reserve(maxEntities);

    float lastSubStepTime = 0.02f;
    std::vector<double> updateTimesInMs;

    double entityInstantiationRate = static_cast<double>(benchmarkTimeInSeconds) * 0.6f
                                     / static_cast<double>(maxEntities);
    auto lastEntityInstantiation = std::chrono::steady_clock::now() -
                                   std::chrono::seconds(static_cast<long>(entityInstantiationRate) * 2);

    const auto bStart = std::chrono::steady_clock::now();

#if !HEADLESS
    while (window.isOpen())
    {
        sf::Event event{};
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                window.close();
                benchmarkTimeInSeconds = 0.0;
            }
        }
#endif
        while (std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - bStart).count() /
               1000 < benchmarkTimeInSeconds)
        {
#if !HEADLESS
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                {
                    window.close();
                    benchmarkTimeInSeconds = 0.0;
                }
            }
#endif

            const auto start = std::chrono::steady_clock::now();

            const auto diffInS = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(
                    start - lastEntityInstantiation).count()) / 1e6;

            if (diffInS > entityInstantiationRate)
            {
                float offset = static_cast<float>(entities.size()) / static_cast<float>(maxEntities) * 10.0f;
                entities.emplace_back(instantiationPos.x - offset, instantiationPos.y);
                lastEntityInstantiation = start;
            }

            for (uint16_t i = 0; i < subStepCount; ++i)
            {
                const auto stepStart = std::chrono::steady_clock::now();
                for (size_t j = 0; j < entities.size(); ++j)
                {
                    Entity& entity = entities[j];
                    // apply gravity
                    entity.accelerate(gravity);
                    // apply collisions
                    for (size_t k = j + 1; k < entities.size(); ++k)
                        resolveCollision(entity, entities[k]);
                    // apply constraint
                    entity.constrain(constraint);
                    // update entity
                    entity.update(lastSubStepTime);
                }
                float stepTimeInS = static_cast<float>(std::chrono::duration_cast<std::chrono::nanoseconds>(
                        std::chrono::steady_clock::now() - start).count()) / 1e9f;
                lastSubStepTime = stepTimeInS;
            }

#if !HEADLESS
            window.clear(sf::Color::White);

            // render constraint
            sf::RectangleShape constraintBackground{{constraint.top, constraint.right}};
            constraintBackground.setOrigin(0.0f, constraint.top);
            constraintBackground.setFillColor(sf::Color::Black);
            constraintBackground.setPosition(0, constraint.top);
            window.draw(constraintBackground);

            // render entities
            sf::CircleShape circle{1.0f};
            circle.setPointCount(32);
            circle.setOrigin(1.0f, 1.0f);
            for (const auto& entity: entities)
            {
                circle.setPosition({entity.position.x, entity.position.y});
                circle.setScale(entityRadius, entityRadius);
                circle.setFillColor({entity.color.r, entity.color.g, entity.color.b});
                window.draw(circle);
            }

            window.display();
#endif

            double timeInMs = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::steady_clock::now() - start).count()) / 1e6;

            updateTimesInMs.push_back(timeInMs);
        }

#if !HEADLESS
    }
#endif

    double avgUpdateTimeInMs = std::reduce(updateTimesInMs.begin(), updateTimesInMs.end()) /
                               static_cast<double>(updateTimesInMs.size());
    std::cout << "Average Update Time in milliseconds: "
              << avgUpdateTimeInMs << std::endl;
    std::cout << "Average Updates per second: " << 1000.0 / avgUpdateTimeInMs << std::endl;
}