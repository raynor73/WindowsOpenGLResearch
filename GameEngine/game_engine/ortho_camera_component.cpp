#include "ortho_camera_component.h"
#include <game_engine/transformation_component.h>
#include <game_engine/game_object.h>
#include <game_engine/constants.h>
#include <game_engine/service_locator.h>
#include <sstream>
#include <exception>
#include <glm/gtc/matrix_transform.hpp>

using namespace GameEngine;
using namespace std;

const string OrthoCameraComponent::TYPE_NAME = "OrthoCameraComponent";

glm::mat4 OrthoCameraComponent::calculateViewMatrix() {
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

glm::mat4 OrthoCameraComponent::calculateProjectionMatrix() {
    if (isDisplayInfoUpdated()) { // TODO Detect not only display info change but also change of viewport as for Perspective Camera
        auto unitsConverter = m_serviceLocator->unitsConverter();
        m_projectionMatrix = glm::ortho(
            unitsConverter->complexValueToPixels(m_left),
            unitsConverter->complexValueToPixels(m_right),
            unitsConverter->complexValueToPixels(m_bottom),
            unitsConverter->complexValueToPixels(m_top),
            m_zNear,
            m_zFar
        );
    }

    return m_projectionMatrix;
}

shared_ptr<GameObjectComponent> OrthoCameraComponent::clone() {
    auto clone = make_shared<OrthoCameraComponent>(
        m_serviceLocator,
        m_clearColor,
        m_layerNames,
        m_left,
        m_top,
        m_right,
        m_bottom,
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
