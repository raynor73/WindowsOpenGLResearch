#include "free_fly_camera_controller.h"
#include <glm/gtc/quaternion.hpp>
#include <game_engine/constants.h>
#include <game_engine/logger.h>
#include <sstream>

using namespace GameEngine;
using namespace std;

FreeFlyCameraController::FreeFlyCameraController(
    std::shared_ptr<GameEngine::ServiceLocator> serviceLocator,
    std::shared_ptr<GameEngine::TransformationComponent> cameraTransform
) : m_serviceLocator(serviceLocator),
    m_cameraTransform(cameraTransform)
{
    m_cameraRotationSensitivity = 90 / m_serviceLocator->renderingWindowInfoProvider()->width(); // 45 degrees per screen half-width mouse movement
}

void FreeFlyCameraController::update(float dt)
{
    if (!m_isEnabled) {
        return;
    }

    auto mouseInput = m_serviceLocator->mouseInput();
    if (abs(mouseInput->dx()) > FLT_EPSILON || abs(mouseInput->dy()) > FLT_EPSILON) {
        m_cameraAngleX += glm::radians(-mouseInput->dy() * m_cameraRotationSensitivity);
        m_cameraAngleY += glm::radians(-mouseInput->dx() * m_cameraRotationSensitivity);

        updateRotation();
    }

    if (m_serviceLocator->keyboardInput()->isKeyPressed(KeyboardInput::KEY_W)) {
        auto rotation = m_cameraTransform->rotation();
        glm::vec3 movement =
            rotation *
            Constants::DEFAULT_FORWARD_DIRECTION *
            m_cameraMovementMaxSpeed *
            dt;
        m_cameraTransform->setPosition(m_cameraTransform->position() + movement);
    }

    if (m_serviceLocator->keyboardInput()->isKeyPressed(KeyboardInput::KEY_S)) {
        auto rotation = m_cameraTransform->rotation();
        glm::vec3 movement =
            rotation *
            Constants::DEFAULT_FORWARD_DIRECTION *
            -m_cameraMovementMaxSpeed *
            dt;
        m_cameraTransform->setPosition(m_cameraTransform->position() + movement);
    }

    if (m_serviceLocator->keyboardInput()->isKeyPressed(KeyboardInput::KEY_D)) {
        auto rotation = m_cameraTransform->rotation();
        glm::vec3 strafe =
            rotation *
            glm::cross(Constants::DEFAULT_FORWARD_DIRECTION, Constants::CAMERA_UP_DIRECTION) *
            m_cameraMovementMaxSpeed *
            dt;
        m_cameraTransform->setPosition(m_cameraTransform->position() + strafe);
    }

    if (m_serviceLocator->keyboardInput()->isKeyPressed(KeyboardInput::KEY_A)) {
        auto rotation = m_cameraTransform->rotation();
        glm::vec3 strafe =
            rotation *
            glm::cross(Constants::DEFAULT_FORWARD_DIRECTION, Constants::CAMERA_UP_DIRECTION) *
            -m_cameraMovementMaxSpeed *
            dt;
        m_cameraTransform->setPosition(m_cameraTransform->position() + strafe);
    }
}

void FreeFlyCameraController::updateRotation()
{
    auto rotation = glm::rotate(
        glm::identity<glm::quat>(),
        m_cameraAngleY,
        glm::vec3(0, 1, 0)
    );
    rotation = glm::rotate(
        rotation,
        m_cameraAngleX,
        glm::vec3(1, 0, 0)
    );
    m_cameraTransform->setRotation(rotation);
}
