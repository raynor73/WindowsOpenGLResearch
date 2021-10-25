#pragma once

#include <game_engine/physics_engine.h>
#include <game_engine/without_generated_methods.h>

namespace GameEngine
{
class BulletPhysicsEngine : public PhysicsEngine, public WithoutGeneratedMethods
{
public:
    virtual void setGravity(const glm::vec3& gravity) override;

    virtual void setPosition(const std::string& rigidBodyName, const glm::vec3& position) override;
    virtual void setRotation(const std::string& rigidBodyName, const glm::quat& rotation) override;

    virtual void addForce(const std::string& rigidBodyName, const glm::vec3& force) override;
    virtual void addTorque(const std::string& rigidBodyName, const glm::vec3& torque) override;

    virtual void setVelocityDirectly(const std::string& rigidBodyName, const glm::vec3& velocity) override;
    virtual void setVelocityViaMotor(const std::string& rigidBodyName, const glm::vec3& velocity) override;

    virtual void setAngularVelocityDirectly(const std::string& rigidBodyName, const glm::vec3& angularVelocity) override;
    virtual void setAngularVelocityViaMotor(const std::string& rigidBodyName, const glm::vec3& velocity) override;

    virtual void setRigidBodyEnabled(const std::string& rigidBodyName, bool isEnabled) override;

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
    ) override;

    virtual void createSphereRigidBody(
        std::shared_ptr<GameObject> gameObject,
        std::string name,
        std::optional<float> massValue,
        float radius,
        const glm::vec3& position,
        const glm::quat& rotation,
        const glm::vec3& maxMotorForce,
        const glm::vec3& maxMotorTorque
    ) override;

    virtual void createBoxRigidBody(
        std::shared_ptr<GameObject> gameObject,
        std::string name,
        std::optional<float> massValue,
        const glm::vec3& size,
        const glm::vec3& position,
        const glm::quat& rotation,
        const glm::vec3& maxMotorForce,
        const glm::vec3& maxMotorTorque
    ) override;

    virtual void createCharacterCapsuleRigidBody(
        std::shared_ptr<GameObject> gameObject,
        std::string name,
        std::optional<float> massValue,
        float radius,
        float length,
        const glm::vec3& position,
        const glm::vec3& maxMotorForce
    ) override;

    virtual void createTriMeshRigidBody(
        std::shared_ptr<GameObject> gameObject,
        std::string name,
        const Mesh& mesh,
        std::optional<float> massValue,
        const glm::vec3& position,
        const glm::quat& rotation
    ) override;

    virtual void removeRigidBody(const std::string& rigidBodyName) override;

    virtual void update(float dt) override;

    virtual void getRigidBodyRotationAndPosition(
        const std::string& rigidBodyName,
        glm::mat4x4& destRotationMatrix,
        glm::vec3& destPosition
    ) override;

    virtual glm::vec3 getRigidBodyVelocity(const std::string& rigidBodyName) override;

    virtual void setRigidBodyFriction(const std::string& rigidBodyName, float friction) override;

    virtual void reset() override;
};
}
