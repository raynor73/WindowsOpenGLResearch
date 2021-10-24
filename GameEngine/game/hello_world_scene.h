#pragma once

#include <game_engine/scene.h>
#include <game/fps_calculator.h>
#include <memory>
#include <game/free_fly_camera_controller.h>

class HelloWorldScene : public GameEngine::Scene
{
    float m_prevFpsValue = NAN;
    FpsCalculator m_fpsCalculator;
    std::shared_ptr<FreeFlyCameraController> m_freeFlyCameraController;

public:
    HelloWorldScene(std::shared_ptr<GameEngine::ServiceLocator> serviceLocator) : Scene(serviceLocator) {}

    virtual void start() override;
    virtual void update() override;

private:
    float m_angle = 0;
};

