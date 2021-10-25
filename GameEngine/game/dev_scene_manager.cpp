#include "dev_scene_manager.h"
#include <game/hello_world_scene.h>
#include <game_engine/service_locator.h>

using namespace GameEngine;
using namespace std;

void DevSceneManager::requestHelloWorldSceneStart()
{
    m_serviceLocator->physicsEngine()->reset();
    m_serviceLocator->renderingEngine()->reset();

    m_activeScene = make_shared<HelloWorldScene>(m_serviceLocator);
    m_serviceLocator->sceneHierarchyLoader()->loadHierarchyIntoScene("scenes/hello_world_scene.json", *m_activeScene);
    m_activeScene->start();
}
