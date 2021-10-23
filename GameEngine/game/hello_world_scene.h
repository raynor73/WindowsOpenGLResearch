#pragma once

#include <game_engine/scene.h>
#include <game/fps_calculator.h>

class HelloWorldScene : public GameEngine::Scene
{
    float m_prevFpsValue = NAN;
    FpsCalculator m_fpsCalculator;

public:
    HelloWorldScene(std::shared_ptr<GameEngine::ServiceLocator> serviceLocator) : Scene(serviceLocator) {}

    virtual void update() override;
};

