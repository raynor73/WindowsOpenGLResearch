#include "camera_components_manager.h"

using namespace GameEngine;
using namespace std;

shared_ptr<PerspectiveCameraComponent> GameEngine::CameraComponentsManager::createPerspectiveCamera(
    shared_ptr<ServiceLocator> serviceLocator,
    const glm::vec4& clearColor,
    const vector<string>& layerNames, 
    float fov,
    float zNear, 
    float zFar, 
    int order
) {
    auto camera = shared_ptr<PerspectiveCameraComponent>(
        new PerspectiveCameraComponent(
            serviceLocator,
            clearColor,
            layerNames,
            fov,
            zNear,
            zFar,
            order
        )
    );
    m_cameras.insert(camera);
    return camera;
}

void GameEngine::CameraComponentsManager::releasePerspectiveCamera(shared_ptr<PerspectiveCameraComponent> camera)
{
    m_cameras.erase(camera);
}

void GameEngine::CameraComponentsManager::reset()
{
    m_cameras.clear();
}
