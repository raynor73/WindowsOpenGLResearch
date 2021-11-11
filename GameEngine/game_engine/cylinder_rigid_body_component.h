#pragma once

#include <game_engine/game_object_component.h>
#include <game_engine/rigid_body_component.h>
#include <optional>

namespace GameEngine
{
class CylinderRigidBodyComponent : public RigidBodyComponent
{
    std::optional<float> m_massValue;
    float m_radius;
    float m_length;

public:
    static const std::string TYPE_NAME;

    CylinderRigidBodyComponent(
        std::weak_ptr<PhysicsEngine> physicsEngine,
        std::optional<float> massValue,
        float radius,
        float length
    );

    virtual const std::string& typeName() const override { return CylinderRigidBodyComponent::TYPE_NAME; }

    virtual std::shared_ptr<GameObjectComponent> clone() override;
};
}

