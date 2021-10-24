#pragma once

#include <game_engine/service_locator.h>
#include <game_engine/transformation_component.h>
#include <memory>

class FreeFlyCameraController
{
    std::shared_ptr<GameEngine::ServiceLocator> m_serviceLocator;
    std::shared_ptr<GameEngine::TransformationComponent> m_cameraTransform;

    float m_cameraAngleX = 0;
    float m_cameraAngleY = 0;

    bool m_isEnabled = true;

    float m_cameraMovementMaxSpeed = 1.0f; // units per second

public:
    FreeFlyCameraController(
        std::shared_ptr<GameEngine::ServiceLocator> serviceLocator,
        std::shared_ptr<GameEngine::TransformationComponent> cameraTransform
    ) : m_serviceLocator(serviceLocator),
        m_cameraTransform(cameraTransform)
    {}
    
    void setEnabled(bool isEnabled) { m_isEnabled = isEnabled; }

    float cameraAngleX() const { return m_cameraAngleX; }
    void setCameraAngleX(float cameraAngleX) { m_cameraAngleX = cameraAngleX; updateRotation(); };

    float cameraAngleY() const { return m_cameraAngleY; }
    void setCameraAngleY(float cameraAngleY) { m_cameraAngleY = cameraAngleY; updateRotation(); }

    void update(float dt);

private:
    void updateRotation();
};
