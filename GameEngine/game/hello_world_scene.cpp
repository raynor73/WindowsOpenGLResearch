#include "hello_world_scene.h"
#include <cmath>
#include <game_engine/service_locator.h>
#include <game_engine/logger.h>
#include <sstream>
#include <game_engine/transformation_component.h>
#include <game_engine/utils.h>

using namespace GameEngine;
using namespace std;

void HelloWorldScene::start()
{
    m_freeFlyCameraController = make_shared<FreeFlyCameraController>(
        m_serviceLocator, 
        m_gameObjectsMap["sceneCamera"]->findComponent<TransformationComponent>()
    );
}

void HelloWorldScene::update()
{
    Scene::update();
    float dt = m_serviceLocator->timeManager()->deltaTime();

    m_freeFlyCameraController->update(dt);

    m_angle += dt * 60;
    auto m_boxTransform = m_gameObjectsMap["box"]->findComponent<TransformationComponent>();
    m_boxTransform->setRotation(
        Utils::eulerZXY(glm::radians(m_angle), glm::radians(m_angle), glm::radians(m_angle))
    );

    m_fpsCalculator.update(dt);
    float fpsValue = m_fpsCalculator.fps();
    if (isnan(m_prevFpsValue) || abs(m_prevFpsValue - fpsValue) > FLT_EPSILON) {
        stringstream ss;
        ss << fpsValue;
        L::d("FPS", ss.str());

        m_prevFpsValue = fpsValue;
    }

    if (m_serviceLocator->keyboardInput()->isKeyPressed(KeyboardInput::KEY_ESC)) {
        m_serviceLocator->app()->requestExit();
    }
}
