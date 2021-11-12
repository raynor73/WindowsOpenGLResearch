#pragma once

#include <game_engine/physics_engine.h>
#include <game_engine/without_generated_methods.h>
#include <string>
#include <memory>
#include <exception>
#include <unordered_map>
#include <bullet/btBulletDynamicsCommon.h>
#include <game_engine/rigid_body_component.h>
#include <physics_engine/sphere_dynamic_rigid_body_allocated_objects.h>
#include <physics_engine/tri_mesh_static_rigid_body_allocated_objects.h>
#include <physics_engine/cylinder_rigid_body_allocated_objects.h>
#include <variant>

namespace GameEngine
{
typedef std::variant<
    SphereDynamicRigidBodyAllocatedObjects*,
    TriMeshStaticRigidBodyAllocatedObjects*,
    CylinderRigidBodyAllocatedObjects*
> btAllocatedObjectsContainer;

class BulletPhysicsEngine : public PhysicsEngine, public WithoutGeneratedMethods
{
    btDefaultCollisionConfiguration* m_collisionConfiguration;
    btCollisionDispatcher* m_dispatcher;
    btDbvtBroadphase* m_overlappingPairCache;
    btSequentialImpulseConstraintSolver* m_solver;
    btDiscreteDynamicsWorld* m_dynamicsWorld;

    std::unordered_map<btCollisionObject*, RigidBodyComponent*> m_btObjectsToRigidBodyComponentMap;
    std::unordered_map<RigidBodyComponent*, btAllocatedObjectsContainer> m_rigidBodyComponentToBtObjectsMap;

public:
    BulletPhysicsEngine();
    virtual ~BulletPhysicsEngine() override;

    virtual void setGravity(const glm::vec3& gravity) override;

    virtual void setPosition(RigidBodyComponent* rigidBodyComponent, const glm::vec3& position) override;
    virtual void setRotation(RigidBodyComponent* rigidBodyComponent, const glm::quat& rotation) override;

    virtual void addForce(RigidBodyComponent* rigidBodyComponent, const glm::vec3& force) override;
    /*
    virtual void addTorque(const std::string& rigidBodyName, const glm::vec3& torque) override;

    virtual void setVelocityDirectly(const std::string& rigidBodyName, const glm::vec3& velocity) override;
    virtual void setVelocityViaMotor(const std::string& rigidBodyName, const glm::vec3& velocity) override;

    virtual void setAngularVelocityDirectly(const std::string& rigidBodyName, const glm::vec3& angularVelocity) override;
    virtual void setAngularVelocityViaMotor(const std::string& rigidBodyName, const glm::vec3& velocity) override;*/

    virtual void setRigidBodyEnabled(RigidBodyComponent* rigidBodyComponent, bool isEnabled) override;

    virtual void createCylinderRigidBody(
        RigidBodyComponent* rigidBodyComponent,
        std::optional<float> massValue,
        float radius,
        float length,
        const glm::vec3& position,
        const glm::quat& rotation
    ) override;

    virtual void createSphereRigidBody(
        RigidBodyComponent* rigidBodyComponent,
        std::optional<float> massValue,
        float radius,
        const glm::vec3& position,
        const glm::quat& rotation
    ) override;
    
    virtual void createBoxRigidBody(
        RigidBodyComponent* rigidBodyComponent,
        std::optional<float> massValue,
        const glm::vec3& size,
        const glm::vec3& position,
        const glm::quat& rotation
    ) override;

    /*
    virtual void createCharacterCapsuleRigidBody(
        std::shared_ptr<GameObject> gameObject,
        std::string name,
        std::optional<float> massValue,
        float radius,
        float length,
        const glm::vec3& position,
        const glm::vec3& maxMotorForce
    ) override;*/

    virtual void createTriMeshRigidBody(
        RigidBodyComponent* rigidBodyComponent,
        const Mesh& mesh,
        std::optional<float> massValue,
        const glm::vec3& position,
        const glm::quat& rotation
    ) override;

    virtual void removeRigidBody(RigidBodyComponent* rigidBodyComponent) override;

    virtual void update(float dt) override;

    virtual void getRigidBodyRotationAndPosition(
        RigidBodyComponent* rigidBodyComponent,
        glm::mat4x4& destRotationMatrix,
        glm::vec3& destPosition
    ) override;

    virtual glm::vec3 getRigidBodyVelocity(RigidBodyComponent* rigidBodyComponent) override;

    virtual void setRigidBodyFriction(RigidBodyComponent* rigidBodyComponent, float friction) override;

    virtual void reset() override;

    virtual bool isKinematic(RigidBodyComponent* rigidBodyComponent) override;

    friend void tickCallback(btDynamicsWorld* world, btScalar timeStep);

private:
    void initBulletPhysics();
    void deinitBulletPhysics();

    void removeAllRigidBodies();

    btAllocatedObjectsContainer getBtObjects(RigidBodyComponent* rigidBodyComponent) const;

    static btVector3 glmVec3ToBtVector3(const glm::vec3& vector) {
        return btVector3(vector.x, vector.y, vector.z);
    }

    static btQuaternion glmQuatToBtQuaternion(const glm::quat& quaternion) {
        return btQuaternion(quaternion.x, quaternion.y, quaternion.z, quaternion.w);
    }

    static glm::vec3 btVector3ToGlmVec3(const btVector3& vector) {
        return glm::vec3(vector.x(), vector.y(), vector.z());
    }

    static glm::quat btQuaternionToGlmQuat(const btQuaternion& quaternion) {
        return glm::quat(quaternion.w(), quaternion.x(), quaternion.y(), quaternion.z());
    }
};
}
