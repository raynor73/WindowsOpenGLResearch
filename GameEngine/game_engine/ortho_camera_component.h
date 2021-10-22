#pragma once

#include <utility>
#include <memory>
#include <glm/gtc/matrix_transform.hpp>
#include <game_engine/camera_component.h>
#include <game_engine/rendering_window_info_update_detector.h>

namespace GameEngine
{
class ServiceLocator;

class OrthoCameraComponent : public CameraComponent, RenderingWindowInfoUpdateDetector
{
    std::shared_ptr<ServiceLocator> m_serviceLocator;

    ComplexValue m_left;
    ComplexValue m_top;
    ComplexValue m_right;
    ComplexValue m_bottom;

    glm::mat4 m_projectionMatrix = glm::identity<glm::mat4>();

public:
    static const std::string TYPE_NAME;

    explicit OrthoCameraComponent(
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
    ) : CameraComponent(clearColor, layerNames, zNear, zFar, order),
        RenderingWindowInfoUpdateDetector(serviceLocator),
        m_serviceLocator(serviceLocator)
    {}

    glm::mat4 calculateViewMatrix() override;
    glm::mat4 calculateProjectionMatrix() override;
    virtual const std::string& typeName() const override { return OrthoCameraComponent::TYPE_NAME; }
    virtual std::shared_ptr<GameObjectComponent> clone() override;
};
}
