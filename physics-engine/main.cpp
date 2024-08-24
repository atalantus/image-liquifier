#include <iostream>
#include <chrono>
#include <vector>
#include <numeric>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <sstream>

#include "engine.hpp"

#define HEADLESS 0

uint64_t maxEntities = 4000;
uint16_t subStepCount = 8;

uint32_t benchmarkTimeInSeconds = 60;

BoxConstraint constraint = {500, 0.0f, 500, 0.0f};
Vec2 instantiationPos = {constraint.right * 0.5f, constraint.top * 0.5f};
float instantiationSpeed = 1200.0f;

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

    Engine engine;
    engine.setUpdateRate(60, 8)
            .setWorldConstraint(constraint);

    std::vector<double> updateTimesInMs;

    double entityInstantiationRate = 0.06;
    auto lastEntityInstantiation = std::chrono::steady_clock::now() -
                                   std::chrono::seconds(static_cast<long>(entityInstantiationRate) * 2);

    const auto bStart = std::chrono::steady_clock::now();
    float timeInSeconds = 0.0f;

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
        while (/*timeInSeconds < benchmarkTimeInSeconds*/ true)
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

            if (diffInS >= entityInstantiationRate && engine.entityCount < maxEntities)
            {
                float angle = sinf(timeInSeconds) + M_PI_2f;
                Vec2 initialVelocity = {cosf(angle), sinf(angle)};
                engine.instantiateEntity({instantiationPos.x, instantiationPos.y},
                                         initialVelocity * instantiationSpeed,
                                         Color(255, 0, 255));
                lastEntityInstantiation = start;
            }

            engine.update();

            double updateTimeInMs = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::steady_clock::now() - start).count()) / 1e6;
            timeInSeconds = static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::steady_clock::now() - bStart).count()) / 1000.0f;

#if !HEADLESS
            window.clear(sf::Color::White);

            // render info text
            sf::Text execTimeText;
            execTimeText.setFont(font);
            std::stringstream ss;
            ss << "Elapsed Time: " << timeInSeconds << " s";
            execTimeText.setString(ss.str());
            execTimeText.setCharacterSize(16);
            execTimeText.setFillColor(sf::Color::Black);
            execTimeText.setPosition(2, 2);
            window.draw(execTimeText);
            sf::Text entityCountText;
            entityCountText.setFont(font);
            std::stringstream ss3;
            ss3 << "Entity Count: " << engine.entityCount;
            entityCountText.setString(ss3.str());
            entityCountText.setCharacterSize(16);
            entityCountText.setFillColor(sf::Color::Black);
            entityCountText.setPosition(2, 24);
            window.draw(entityCountText);
            sf::Text updateTimeText;
            updateTimeText.setFont(font);
            std::stringstream ss1;
            ss1 << "Update Time: " << updateTimeInMs << " ms";
            updateTimeText.setString(ss1.str());
            updateTimeText.setCharacterSize(16);
            updateTimeText.setFillColor(sf::Color::Black);
            updateTimeText.setPosition(2, 46);
            window.draw(updateTimeText);
            sf::Text updatesPerSecondText;
            updatesPerSecondText.setFont(font);
            std::stringstream ss2;
            ss2 << "Updates per second: " << 1000.0 / updateTimeInMs;
            updatesPerSecondText.setString(ss2.str());
            updatesPerSecondText.setCharacterSize(16);
            updatesPerSecondText.setFillColor(sf::Color::Black);
            updatesPerSecondText.setPosition(2, 68);
            window.draw(updatesPerSecondText);


            // render constraint
            sf::RectangleShape constraintBackground{{constraint.right, constraint.top}};
            constraintBackground.setOrigin(constraintBackground.getSize().x / 2.0f,
                                           constraintBackground.getSize().y / 2.0f);
            constraintBackground.setFillColor(sf::Color::Black);
            constraintBackground.setPosition(static_cast<float>(windowWidth) / 2.0f,
                                             static_cast<float>(windowHeight) / 2.0f);
            window.draw(constraintBackground);

            float offsetX = static_cast<float>(windowWidth) / 2.0f - constraintBackground.getSize().x / 2.0f;
            float offsetY = static_cast<float>(windowHeight) / 2.0f - constraintBackground.getSize().y / 2.0f;

            // render entities
            sf::CircleShape circle{1.0f};
            circle.setPointCount(32);
            circle.setOrigin(1.0f, 1.0f);
            circle.setPosition({offsetX + instantiationPos.x,
                                offsetY + instantiationPos.y});
            circle.setScale(4, 4);
            circle.setFillColor(sf::Color::Red);
            window.draw(circle);
            for (uint32_t i = 0; i < engine.entityCount; ++i)
            {
                const auto& entity = engine.entities[i];
                circle.setPosition({offsetX + entity.position.x,
                                    offsetY + entity.position.y});
                circle.setScale(entityRadius, entityRadius);
                circle.setFillColor({entity.color.r, entity.color.g, entity.color.b});
                window.draw(circle);
            }

            window.display();
#endif

            updateTimesInMs.push_back(updateTimeInMs);
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