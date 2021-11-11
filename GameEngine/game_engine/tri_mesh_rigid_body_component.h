#pragma once

#include <game_engine/game_object_component.h>
#include <game_engine/rigid_body_component.h>
#include <optional>
#include <game_engine/mesh.h>

namespace GameEngine
{
class TriMeshRigidBodyComponent : public RigidBodyComponent
{
    std::optional<float> m_massValue;
    Mesh m_mesh;

public:
    static const std::string TYPE_NAME;

    TriMeshRigidBodyComponent(
        std::weak_ptr<PhysicsEngine> physicsEngine,
        std::optional<float> massValue,
        const Mesh& mesh
    );

    virtual const std::string& typeName() const override { return TriMeshRigidBodyComponent::TYPE_NAME; }

    virtual std::shared_ptr<GameObjectComponent> clone() override;
};
}
