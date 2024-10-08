#include <iostream>
#include <chrono>
#include <vector>
#include <numeric>
#include <SFML/Graphics.hpp>
#include <sstream>

#include "engine.hpp"

#define HEADLESS 0

uint64_t maxEntities = 1000;
uint16_t subStepCount = 8;

uint32_t benchmarkTimeInSeconds = 60;

std::vector<Entity> entities;
BoxConstraint constraint = {static_cast<float>(maxEntities) * 0.5f, 0.0f, static_cast<float>(maxEntities) * 0.6f, 0.0f};
Vec2 instantiationPos = {constraint.right * 0.8f, constraint.top * 0.8f};
Vec2 gravity = {0.0f, 1000.0f};

int main()
{

#if !HEADLESS
    sf::Font font;
    if (!font.loadFromFile("./resources/Roboto-Medium.ttf"))
    {
        return 1;
    }
    uint32_t windowWidth = 1000;
    uint32_t windowHeight = 1000;
    sf::ContextSettings settings;
    settings.antialiasingLevel = 1;
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Benchmark", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(false);
    window.setFramerateLimit(60);

    float constraintOffsetX = (static_cast<float>(windowWidth) - constraint.right) / 2.0f;
    float constraintOffsetY = (static_cast<float>(windowHeight) - constraint.top) / 2.0f;
#endif

    entities.reserve(maxEntities);

    float lastSubStepTime = 0.02f;
    std::vector<double> updateTimesInMs;

    double entityInstantiationRate = static_cast<double>(benchmarkTimeInSeconds)
                                     / static_cast<double>(maxEntities);
    auto lastEntityInstantiation = std::chrono::steady_clock::now() -
                                   std::chrono::seconds(static_cast<long>(entityInstantiationRate) * 2);

    const auto bStart = std::chrono::steady_clock::now();
    double bTime = 0.0f;

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
        while (bTime < benchmarkTimeInSeconds)
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
                    start - lastEntityInstantiation).count()) / 1e9;

            if (diffInS > entityInstantiationRate && entities.size() < maxEntities)
            {
                float offset = static_cast<float>(entities.size()) / static_cast<float>(maxEntities) * 50.0f;
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

            double timeInMs = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::steady_clock::now() - start).count()) / 1e6;

            bTime = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::steady_clock::now() - bStart).count() / 1000;

#if !HEADLESS
            window.clear(sf::Color::White);

            // render info text
            sf::Text execTimeText;
            execTimeText.setFont(font);
            std::stringstream ss;
            ss << "Time: " << bTime << " s";
            execTimeText.setString(ss.str());
            execTimeText.setCharacterSize(16);
            execTimeText.setFillColor(sf::Color::Black);
            execTimeText.setPosition(2, 2);
            window.draw(execTimeText);
            sf::Text entityCountText;
            entityCountText.setFont(font);
            std::stringstream ss3;
            ss3 << "Entity Count: " << entities.size();
            entityCountText.setString(ss3.str());
            entityCountText.setCharacterSize(16);
            entityCountText.setFillColor(sf::Color::Black);
            entityCountText.setPosition(2, 24);
            window.draw(entityCountText);
            sf::Text updateTimeText;
            updateTimeText.setFont(font);
            std::stringstream ss1;
            ss1 << "Update Time: " << timeInMs << " ms";
            updateTimeText.setString(ss1.str());
            updateTimeText.setCharacterSize(16);
            updateTimeText.setFillColor(sf::Color::Black);
            updateTimeText.setPosition(2, 46);
            window.draw(updateTimeText);
            sf::Text updatesPerSecondText;
            updatesPerSecondText.setFont(font);
            std::stringstream ss2;
            ss2 << "Updates per second: " << 1000.0 / timeInMs;
            updatesPerSecondText.setString(ss2.str());
            updatesPerSecondText.setCharacterSize(16);
            updatesPerSecondText.setFillColor(sf::Color::Black);
            updatesPerSecondText.setPosition(2, 68);
            window.draw(updatesPerSecondText);


            // render constraint
            sf::RectangleShape constraintBackground{{constraint.top, constraint.right}};;
            constraintBackground.setFillColor(sf::Color::Black);
            constraintBackground.setPosition(constraintOffsetX, constraintOffsetY);
            window.draw(constraintBackground);

            // render entities
            sf::CircleShape circle{1.0f};
            circle.setPointCount(32);
            for (const auto& entity: entities)
            {
                circle.setPosition({constraintOffsetY + entity.position.y,
                                    constraintOffsetX + entity.position.x});
                circle.setScale(entityRadius, entityRadius);
                circle.setFillColor({entity.color.r, entity.color.g, entity.color.b});
                window.draw(circle);
            }

            window.display();
#endif

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