#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>
#include <optional>
#include <game_engine/mesh.h>
#include <game_engine/game_object.h>
#include <game_engine/rigid_body_component.h>

namespace GameEngine
{
class PhysicsEngine
{
public:
    virtual ~PhysicsEngine() = default;

    virtual void setGravity(const glm::vec3& gravity) = 0;

    virtual void setPosition(RigidBodyComponent* rigidBodyComponent, const glm::vec3& position) = 0;
    virtual void setRotation(RigidBodyComponent* rigidBodyComponent, const glm::quat& rotation) = 0;

    virtual void addForce(RigidBodyComponent* rigidBodyComponent, const glm::vec3& force) = 0;
    /*virtual void addTorque(const RigidBodyComponent* rigidBodyComponent, const glm::vec3& torque) = 0;

    virtual void setVelocityDirectly(const RigidBodyComponent* rigidBodyComponent, const glm::vec3& velocity) = 0;
    virtual void setVelocityViaMotor(const RigidBodyComponent* rigidBodyComponent, const glm::vec3& velocity) = 0;

    virtual void setAngularVelocityDirectly(const RigidBodyComponent* rigidBodyComponent, const glm::vec3& angularVelocity) = 0;
    virtual void setAngularVelocityViaMotor(const RigidBodyComponent* rigidBodyComponent, const glm::vec3& velocity) = 0;*/

    virtual void setRigidBodyEnabled(RigidBodyComponent* rigidBodyComponent, bool isEnabled) = 0;

    virtual void createCylinderRigidBody(
        RigidBodyComponent* rigidBodyComponent,
        std::optional<float> massValue,
        float radius,
        float length,
        const glm::vec3& position,
        const glm::quat& rotation
    ) = 0;

    virtual void createSphereRigidBody(
        RigidBodyComponent* rigidBodyComponent,
        std::optional<float> massValue,
        float radius,
        const glm::vec3& position,
        const glm::quat& rotation
    ) = 0;

    virtual void createBoxRigidBody(
        RigidBodyComponent* rigidBodyComponent,
        std::optional<float> massValue,
        const glm::vec3& size,
        const glm::vec3& position,
        const glm::quat& rotation
    ) = 0;

    /*virtual void createCharacterCapsuleRigidBody(
        const RigidBodyComponent* rigidBodyComponent,
        std::optional<float> massValue,
        float radius,
        float length,
        const glm::vec3& position,
        const glm::vec3& maxMotorForce
    ) = 0;*/

    virtual void createTriMeshRigidBody(
        RigidBodyComponent* rigidBodyComponent,
        const Mesh& mesh,
        std::optional<float> massValue,
        const glm::vec3& position,
        const glm::quat& rotation
    ) = 0;

    virtual void removeRigidBody(RigidBodyComponent* rigidBodyComponent) = 0;

    virtual void update(float dt) = 0;

    virtual void getRigidBodyRotationAndPosition(
        RigidBodyComponent* rigidBodyComponent,
        glm::mat4x4& destRotationMatrix,
        glm::vec3& destPosition
    ) = 0;

    virtual glm::vec3 getRigidBodyVelocity(RigidBodyComponent* rigidBodyComponent) = 0;

    virtual void setRigidBodyFriction(RigidBodyComponent* rigidBodyComponent, float friction) = 0;

    virtual void reset() = 0;

    virtual bool isKinematic(RigidBodyComponent* rigidBodyComponent) = 0;
};
}
