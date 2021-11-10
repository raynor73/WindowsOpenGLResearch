#pragma once

#include <game_engine/game_object_component.h>
#include <game_engine/rigid_body_component.h>
#include <optional>

namespace GameEngine
{
class SphereRigidBodyComponent : public RigidBodyComponent
{
    std::optional<float> m_massValue;
    float m_radius;

public:
    static const std::string TYPE_NAME;

    SphereRigidBodyComponent(
        std::weak_ptr<PhysicsEngine> physicsEngine,
        std::optional<float> massValue,
        float radius
    );

    virtual const std::string& typeName() const override { return SphereRigidBodyComponent::TYPE_NAME; }

    virtual std::shared_ptr<GameObjectComponent> clone() override;
};
}
