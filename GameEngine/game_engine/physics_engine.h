#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>
#include <optional>
#include <game_engine/mesh.h>
#include <game_engine/game_object.h>

namespace GameEngine
{
class PhysicsEngine
{
public:
    virtual ~PhysicsEngine() = default;

    virtual void setGravity(const glm::vec3& gravity) = 0;

    virtual void setPosition(const std::string& rigidBodyName, const glm::vec3& position) = 0;
    virtual void setRotation(const std::string& rigidBodyName, const glm::quat& rotation) = 0;

    virtual void addForce(const std::string& rigidBodyName, const glm::vec3& force) = 0;
    virtual void addTorque(const std::string& rigidBodyName, const glm::vec3& torque) = 0;

    virtual void setVelocityDirectly(const std::string& rigidBodyName, const glm::vec3& velocity) = 0;
    virtual void setVelocityViaMotor(const std::string& rigidBodyName, const glm::vec3& velocity) = 0;

    virtual void setAngularVelocityDirectly(const std::string& rigidBodyName, const glm::vec3& angularVelocity) = 0;
    virtual void setAngularVelocityViaMotor(const std::string& rigidBodyName, const glm::vec3& velocity) = 0;

    virtual void setRigidBodyEnabled(const std::string& rigidBodyName, bool isEnabled) = 0;

    virtual void createCylinderRigidBody(
        std::shared_ptr<GameObject> gameObject,
        std::string name,
        std::optional<float> massValue,
        float radius,
        float length,
        const glm::vec3& position,
        const glm::quat& rotation,
        const glm::vec3& maxMotorForce,
        const glm::vec3& maxMotorTorque
    ) = 0;

    virtual void createSphereRigidBody(
        std::shared_ptr<GameObject> gameObject,
        std::string name,
        std::optional<float> massValue,
        float radius,
        const glm::vec3& position,
        const glm::quat& rotation,
        const glm::vec3& maxMotorForce,
        const glm::vec3& maxMotorTorque
    ) = 0;

    virtual void createBoxRigidBody(
        std::shared_ptr<GameObject> gameObject,
        std::string name,
        std::optional<float> massValue,
        const glm::vec3& size,
        const glm::vec3& position,
        const glm::quat& rotation,
        const glm::vec3& maxMotorForce,
        const glm::vec3& maxMotorTorque
    ) = 0;

    virtual void createCharacterCapsuleRigidBody(
        std::shared_ptr<GameObject> gameObject,
        std::string name,
        std::optional<float> massValue,
        float radius,
        float length,
        const glm::vec3& position,
        const glm::vec3& maxMotorForce
    ) = 0;

    virtual void createTriMeshRigidBody(
        std::shared_ptr<GameObject> gameObject,
        std::string name,
        const Mesh& mesh,
        std::optional<float> massValue,
        const glm::vec3& position,
        const glm::quat& rotation
    ) = 0;

    virtual void removeRigidBody(const std::string& rigidBodyName) = 0;

    virtual void update(float dt) = 0;

    virtual void getRigidBodyRotationAndPosition(
        const std::string& rigidBodyName,
        glm::mat4x4& destRotationMatrix,
        glm::vec3& destPosition
    ) = 0;

    virtual glm::vec3 getRigidBodyVelocity(const std::string& rigidBodyName) = 0;

    virtual void setRigidBodyFriction(const std::string& rigidBodyName, float friction) = 0;

    virtual void reset() = 0;
};
}
