#pragma once

#include <game_engine/scene.h>

class HelloWorldScene : public GameEngine::Scene
{
public:
    HelloWorldScene(std::shared_ptr<GameEngine::ServiceLocator> serviceLocator) : Scene(serviceLocator) {}

    virtual void update() override;
};

