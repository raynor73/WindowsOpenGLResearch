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

std::shared_ptr<OrthoCameraComponent> GameEngine::CameraComponentsManager::createOrthoCamera(
    std::shared_ptr<ServiceLocator> serviceLocator,
    const glm::vec4& clearColor,
    const std::vector<std::string>& layerNames,
    ComplexValue left,
    ComplexValue top,
    ComplexValue right,
    ComplexValue bottom,
    float zNear,
    float zFar,
    int order
) {
    auto camera = shared_ptr<OrthoCameraComponent>(
        new OrthoCameraComponent(
            serviceLocator,
            clearColor,
            layerNames,
            left,
            top,
            right,
            bottom,
            zNear,
            zFar,
            order
        )
    );
    m_cameras.insert(camera);
    return camera;
}

void GameEngine::CameraComponentsManager::releaseOrthoCamera(std::shared_ptr<OrthoCameraComponent> camera)
{
    m_cameras.erase(camera);
}

void GameEngine::CameraComponentsManager::reset()
{
    m_cameras.clear();
}
