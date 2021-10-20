#include <game_engine/service_locator.h>
#include "scene.h"

using namespace GameEngine;

void Scene::update()
{
    m_serviceLocator->timeManager()->update();
}
