#include "bullet_physics_engine.h"

using namespace GameEngine;
using namespace std;

void tickCallback(btDynamicsWorld* world, btScalar) {
    /*auto physicsEngine = reinterpret_cast<BulletPhysicsEngine*>(world->getWorldUserInfo());
    auto dispatcher = world->getDispatcher();
    auto numberOfManifolds = dispatcher->getNumManifolds();
    for (int i = 0; i < numberOfManifolds; i++) {
        auto manifold = dispatcher->getManifoldByIndexInternal(i);

        auto body0 = static_cast<const btRigidBody*>(manifold->getBody0());
        auto body1 = static_cast<const btRigidBody*>(manifold->getBody1());

        auto gameObject0 = physicsEngine->m_btRigidBodyToGameObjectMap.at(body0);
        auto gameObject1 = physicsEngine->m_btRigidBodyToGameObjectMap.at(body1);

        auto numberOfContacts = manifold->getNumContacts();
        for (int j = 0; j < numberOfContacts; j++) {
            auto contact = manifold->getContactPoint(j);

            {
                Collision collision;
                collision.gameObject = gameObject0;
                collision.position = BulletPhysicsEngine::btVector3ToGlmVec3(contact.m_positionWorldOnB);
                collision.normal = BulletPhysicsEngine::btVector3ToGlmVec3(contact.m_normalWorldOnB);
                collision.depth = contact.m_distance1;

                auto collisionsInfo = gameObject1->findComponent<CollisionsInfoComponent>();
                if (collisionsInfo != nullptr) {
                    collisionsInfo->collisions.push_back(collision);
                }
            }

            {
                Collision collision;
                collision.gameObject = gameObject1;
                collision.position = BulletPhysicsEngine::btVector3ToGlmVec3(contact.m_positionWorldOnB);
                collision.normal = BulletPhysicsEngine::btVector3ToGlmVec3(contact.m_normalWorldOnB);
                collision.depth = contact.m_distance1;

                auto collisionsInfo = gameObject0->findComponent<CollisionsInfoComponent>();
                if (collisionsInfo != nullptr) {
                    collisionsInfo->collisions.push_back(collision);
                }
            }
        }
    }*/
}

BulletPhysicsEngine::BulletPhysicsEngine()
{
    initBulletPhysics();
}

GameEngine::BulletPhysicsEngine::~BulletPhysicsEngine()
{
    deinitBulletPhysics();
}

void BulletPhysicsEngine::reset()
{
    removeAllRigidBodies();
    setGravity(glm::vec3(0));
}

void GameEngine::BulletPhysicsEngine::initBulletPhysics()
{
    m_collisionConfiguration = new btDefaultCollisionConfiguration();
    m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
    m_overlappingPairCache = new btDbvtBroadphase();
    m_solver = new btSequentialImpulseConstraintSolver();
    m_dynamicsWorld = new btDiscreteDynamicsWorld(
        m_dispatcher,
        m_overlappingPairCache, m_solver,
        m_collisionConfiguration
    );

    m_dynamicsWorld->setInternalTickCallback(tickCallback, this);
}

void GameEngine::BulletPhysicsEngine::deinitBulletPhysics()
{
    
}

void GameEngine::BulletPhysicsEngine::removeAllRigidBodies()
{
}

std::shared_ptr<btRigidBody> GameEngine::BulletPhysicsEngine::getRigidBody(const std::string& name) const
{
    return std::shared_ptr<btRigidBody>();
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
