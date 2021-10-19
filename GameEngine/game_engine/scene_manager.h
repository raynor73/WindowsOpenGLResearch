#pragma once

#include <memory>
#include <game_engine/scene.h>

namespace GameEngine
{
class SceneManager
{
public:
    void requestHelloWorldSceneStart();

    Scene* activeScene() { return m_activeScene.get(); };

private:
    std::shared_ptr<Scene> m_activeScene;
};
}
