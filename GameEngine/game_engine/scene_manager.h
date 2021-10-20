#pragma once

#include <memory>
#include <game_engine/scene.h>

namespace GameEngine
{
class ServiceLocator;

class SceneManager
{
public:
    explicit SceneManager(std::shared_ptr<ServiceLocator> serviceLocator) : m_serviceLocator(std::move(serviceLocator)) {}
    virtual ~SceneManager() = default;

    Scene* activeScene() { return m_activeScene.get(); };

protected:
    std::shared_ptr<Scene> m_activeScene;
    std::shared_ptr<ServiceLocator> m_serviceLocator;
};
}
