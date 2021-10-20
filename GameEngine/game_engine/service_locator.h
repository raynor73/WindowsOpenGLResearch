#pragma once

#include <game_engine/scene_manager.h>
#include <game_engine/time_manager.h>
#include <game_engine/rendering_window_info_provider.h>
#include <memory>

namespace GameEngine
{
class ServiceLocator
{
public:
    void provide(std::shared_ptr<SceneManager> sceneManager);
    void provide(std::shared_ptr<TimeManager> timeManager);
    void provide(std::shared_ptr<RenderingWindowInfoProvider> renderingWindowInfoProvider);

    SceneManager* sceneManager();
    TimeManager* timeManager();
    RenderingWindowInfoProvider* renderingWindowInfoProvider();

private:
    std::shared_ptr<SceneManager> m_sceneManager;
    std::shared_ptr<TimeManager> m_timeManager;
    std::shared_ptr<RenderingWindowInfoProvider> m_renderingWindowInfoProvider;
};
}
