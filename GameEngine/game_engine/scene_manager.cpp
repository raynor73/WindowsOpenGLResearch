#include "scene_manager.h"
#include <game/hello_world_scene.h>

using namespace GameEngine;
using namespace std;

void SceneManager::requestHelloWorldSceneStart()
{
    m_activeScene = make_shared<HelloWorldScene>();
}
