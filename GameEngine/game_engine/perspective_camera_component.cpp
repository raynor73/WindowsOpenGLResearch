#include "perspective_camera_component.h"
#include <game_engine/service_locator.h>
#include <game_engine/transformation_component.h>
#include <game_engine/constants.h>

using namespace GameEngine;
using namespace std;

const string PerspectiveCameraComponent::TYPE_NAME = "PerspectiveCameraComponent";

glm::mat4 PerspectiveCameraComponent::calculateViewMatrix() {
    throwErrorIfNoGameObject();

    auto gameObject = m_gameObject.lock();
    auto transform = static_pointer_cast<TransformationComponent>(gameObject->findComponent(TransformationComponent::TYPE_NAME));
    if (transform == nullptr) {
        stringstream ss;
        ss << "No transform component for game object " << gameObject->name() << " found";
        throw domain_error(ss.str());
    }

    glm::vec3 lookAtDirection;
    glm::vec3 up;

    lookAtDirection = Constants::DEFAULT_FORWARD_DIRECTION;
    lookAtDirection = transform->rotation() * lookAtDirection;

    up = Constants::CAMERA_UP_DIRECTION;
    up = transform->rotation() * up;

    return glm::lookAt(transform->position(), transform->position() + lookAtDirection, up);
}

glm::mat4 PerspectiveCameraComponent::calculateProjectionMatrix() {
    auto unitsConverter = m_serviceLocator->unitsConverter();

    float currentViewportWidth = unitsConverter->complexValueToPixels(m_viewportWidth);
    float currentViewportHeight = unitsConverter->complexValueToPixels(m_viewportHeight);
    if (m_lastViewportWidth != currentViewportWidth || m_lastViewportHeight != currentViewportHeight) {
        m_lastViewportWidth = currentViewportWidth;
        m_lastViewportHeight = currentViewportHeight;

        m_projectionMatrix = glm::perspective(
            m_fov,
            currentViewportWidth / currentViewportHeight,
            m_zNear,
            m_zFar
        );
    }

    return m_projectionMatrix;
}

shared_ptr<GameObjectComponent> PerspectiveCameraComponent::clone() {
    auto clone = make_shared<PerspectiveCameraComponent>(
        m_serviceLocator,
        m_clearColor,
        m_layerNames,
        m_fov,
        m_zNear,
        m_zFar,
        m_order
    );
    clone->setEnabled(m_isEnabled);
    clone->setViewportX(m_viewportX);
    clone->setViewportY(m_viewportY);
    clone->setViewportWidth(m_viewportWidth);
    clone->setViewportHeight(m_viewportHeight);
    clone->setShouldClearDepth(m_shouldClearDepth);
    clone->setShouldClearColor(m_shouldClearColor);
    return clone;
}
