#pragma once

#include <glm/glm.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <memory>
#include <game_engine/camera_component.h>

namespace GameEngine
{
class ServiceLocator;
class CameraComponentsManager;

class PerspectiveCameraComponent : public CameraComponent
{
    std::shared_ptr<ServiceLocator> m_serviceLocator;

    float m_fov;
    float m_lastViewportWidth = NAN;
    float m_lastViewportHeight = NAN;

    glm::mat4 m_projectionMatrix = glm::identity<glm::mat4>();

public:
    static const std::string TYPE_NAME;

    virtual glm::mat4 calculateViewMatrix() override;
    virtual glm::mat4 calculateProjectionMatrix() override;

    virtual const std::string& typeName() const override { return TYPE_NAME; }

    virtual std::shared_ptr<GameObjectComponent> clone() override;

private:
    friend class CameraComponentsManager;

    explicit PerspectiveCameraComponent(
        std::shared_ptr<ServiceLocator> serviceLocator,
        const glm::vec4& clearColor,
        const std::vector<std::string>& layerNames,
        float fov,
        float zNear,
        float zFar,
        int order
    ) : CameraComponent(
        clearColor,
        layerNames,
        zNear,
        zFar,
        order
    ),
        m_serviceLocator(serviceLocator),
        m_fov(fov)
    {}
};
}
