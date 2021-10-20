#include "dev_scene_manager.h"
#include <game/hello_world_scene.h>

using namespace GameEngine;
using namespace std;

void DevSceneManager::requestHelloWorldSceneStart()
{
    m_activeScene = make_shared<HelloWorldScene>(m_serviceLocator);
}
