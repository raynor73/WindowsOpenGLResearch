#include "bullet_physics_engine.h"

using namespace GameEngine;
using namespace std;

void BulletPhysicsEngine::reset()
{

}

void GameEngine::BulletPhysicsEngine::setGravity(const glm::vec3& gravity)
{
}

void GameEngine::BulletPhysicsEngine::setPosition(const std::string& rigidBodyName, const glm::vec3& position)
{
}

void GameEngine::BulletPhysicsEngine::setRotation(const std::string& rigidBodyName, const glm::quat& rotation)
{
}

void GameEngine::BulletPhysicsEngine::addForce(const std::string& rigidBodyName, const glm::vec3& force)
{
}

void GameEngine::BulletPhysicsEngine::addTorque(const std::string& rigidBodyName, const glm::vec3& torque)
{
}

void GameEngine::BulletPhysicsEngine::setVelocityDirectly(const std::string& rigidBodyName, const glm::vec3& velocity)
{
}

void GameEngine::BulletPhysicsEngine::setVelocityViaMotor(const std::string& rigidBodyName, const glm::vec3& velocity)
{
}

void GameEngine::BulletPhysicsEngine::setAngularVelocityDirectly(const std::string& rigidBodyName, const glm::vec3& angularVelocity)
{
}

void GameEngine::BulletPhysicsEngine::setAngularVelocityViaMotor(const std::string& rigidBodyName, const glm::vec3& velocity)
{
}

void GameEngine::BulletPhysicsEngine::setRigidBodyEnabled(const std::string& rigidBodyName, bool isEnabled)
{
}

void GameEngine::BulletPhysicsEngine::createCylinderRigidBody(std::shared_ptr<GameObject> gameObject, std::string name, std::optional<float> massValue, float radius, float length, const glm::vec3& position, const glm::quat& rotation, const glm::vec3& maxMotorForce, const glm::vec3& maxMotorTorque)
{
}

void GameEngine::BulletPhysicsEngine::createSphereRigidBody(std::shared_ptr<GameObject> gameObject, std::string name, std::optional<float> massValue, float radius, const glm::vec3& position, const glm::quat& rotation, const glm::vec3& maxMotorForce, const glm::vec3& maxMotorTorque)
{
}

void GameEngine::BulletPhysicsEngine::createBoxRigidBody(std::shared_ptr<GameObject> gameObject, std::string name, std::optional<float> massValue, const glm::vec3& size, const glm::vec3& position, const glm::quat& rotation, const glm::vec3& maxMotorForce, const glm::vec3& maxMotorTorque)
{
}

void GameEngine::BulletPhysicsEngine::createCharacterCapsuleRigidBody(std::shared_ptr<GameObject> gameObject, std::string name, std::optional<float> massValue, float radius, float length, const glm::vec3& position, const glm::vec3& maxMotorForce)
{
}

void GameEngine::BulletPhysicsEngine::createTriMeshRigidBody(std::shared_ptr<GameObject> gameObject, std::string name, const Mesh& mesh, std::optional<float> massValue, const glm::vec3& position, const glm::quat& rotation)
{
}

void GameEngine::BulletPhysicsEngine::removeRigidBody(const std::string& rigidBodyName)
{
}

void GameEngine::BulletPhysicsEngine::update(float dt)
{
}

void GameEngine::BulletPhysicsEngine::getRigidBodyRotationAndPosition(const std::string& rigidBodyName, glm::mat4x4& destRotationMatrix, glm::vec3& destPosition)
{
}

glm::vec3 GameEngine::BulletPhysicsEngine::getRigidBodyVelocity(const std::string& rigidBodyName)
{
    return glm::vec3();
}

void GameEngine::BulletPhysicsEngine::setRigidBodyFriction(const std::string& rigidBodyName, float friction)
{
}
