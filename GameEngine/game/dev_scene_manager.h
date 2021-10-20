#pragma once

#include <game_engine/scene_manager.h>

namespace GameEngine
{
class DevSceneManager : public SceneManager
{
public:
    explicit DevSceneManager(std::shared_ptr<ServiceLocator> serviceLocator) : SceneManager(serviceLocator) {}

    void requestHelloWorldSceneStart();
};
}

