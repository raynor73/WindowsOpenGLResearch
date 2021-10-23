#include "hello_world_scene.h"
#include <cmath>
#include <game_engine/service_locator.h>
#include <game_engine/logger.h>
#include <sstream>

using namespace GameEngine;
using namespace std;

void HelloWorldScene::update()
{
    Scene::update();
    
    m_fpsCalculator.update(m_serviceLocator->timeManager()->deltaTime());
    float fpsValue = m_fpsCalculator.fps();
    if (isnan(m_prevFpsValue) || abs(m_prevFpsValue - fpsValue) > FLT_EPSILON) {
        stringstream ss;
        ss << "FPS: " << fpsValue;
        L::d("!@#", ss.str());

        m_prevFpsValue = fpsValue;
    }
}
