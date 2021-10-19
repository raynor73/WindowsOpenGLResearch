#pragma once

#include <game_engine/scene_manager.h>
#include <memory>

namespace GameEngine
{
class ServiceLocator
{
public:
    void provide(std::shared_ptr<SceneManager> sceneManager);
    SceneManager* sceneManager();

private:
    std::shared_ptr<SceneManager> m_sceneManager;
};
}
