#pragma once 

#include <memory>
#include <set>
#include <game_engine/perspective_camera_component.h>
#include <game_engine/ortho_camera_component.h>
#include <game_engine/without_generated_methods.h>
#include <functional>

namespace GameEngine
{
class ServiceLocator;

class CameraComponentsManager  : public WithoutGeneratedMethods
{
    std::set<std::shared_ptr<CameraComponent>, std::function<bool(std::shared_ptr<CameraComponent>, std::shared_ptr<CameraComponent>)>> m_cameras;
public:
    CameraComponentsManager() : m_cameras([](std::shared_ptr<CameraComponent> lhs, std::shared_ptr<CameraComponent> rhs) { return lhs->order() < rhs->order(); }) {}

    const std::set<std::shared_ptr<CameraComponent>, std::function<bool(std::shared_ptr<CameraComponent>, std::shared_ptr<CameraComponent>)>>& cameras() { return m_cameras; }

    std::shared_ptr<PerspectiveCameraComponent> createPerspectiveCamera(
        std::shared_ptr<ServiceLocator> serviceLocator,
        const glm::vec4& clearColor,
        const std::vector<std::string>& layerNames,
        float fov,
        float zNear,
        float zFar,
        int order
    );

    void releasePerspectiveCamera(std::shared_ptr<PerspectiveCameraComponent> camera);

    std::shared_ptr<OrthoCameraComponent> createOrthoCamera(
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
    );

    void releaseOrthoCamera(std::shared_ptr<OrthoCameraComponent> camera);

    void reset();
};
}
