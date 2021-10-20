#pragma once

#include <game_engine/scene_manager.h>
#include <game_engine/time_manager.h>
#include <memory>

namespace GameEngine
{
class ServiceLocator
{
public:
    void provide(std::shared_ptr<SceneManager> sceneManager);
    void provide(std::shared_ptr<TimeManager> timeManager);

    SceneManager* sceneManager();
    TimeManager* timeManager();

private:
    std::shared_ptr<SceneManager> m_sceneManager;
    std::shared_ptr<TimeManager> m_timeManager;
};
}
