#include "bullet_physics_engine.h"
#include <game_engine/utils.h>
#include <vector>

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

BulletPhysicsEngine::~BulletPhysicsEngine()
{
    deinitBulletPhysics();
}

void BulletPhysicsEngine::reset()
{
    removeAllRigidBodies();
    setGravity(glm::vec3(0));
}

void BulletPhysicsEngine::initBulletPhysics()
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

void BulletPhysicsEngine::deinitBulletPhysics()
{
    removeAllRigidBodies();

    delete m_dynamicsWorld;
    delete m_solver;
    delete m_overlappingPairCache;
    delete m_dispatcher;
    delete m_collisionConfiguration;
}

void BulletPhysicsEngine::removeAllRigidBodies()
{
    vector<RigidBodyComponent*> rigidBodiesToRemove;
    for (auto& entry : m_btRigidBodyToRigidBodyComponentMap) {
        rigidBodiesToRemove.push_back(entry.second);
    }

    for (auto& rigidBodyToRemove : rigidBodiesToRemove) {
        removeRigidBody(rigidBodyToRemove);
    }
}

btRigidBody* BulletPhysicsEngine::getRigidBody(RigidBodyComponent* rigidBodyComponent) const
{
    auto gameObject = rigidBodyComponent->gameObject().lock();
    Utils::throwErrorIfNull(gameObject, []() {
        return "Rigid Object Component has no Game Object";
    });

    if (m_rigidBodyComponentToBtRigidBodyMap.count(rigidBodyComponent) == 0) {
        stringstream ss;
        ss << "Bullet Rigid Body for Game Object " << gameObject->name() << " not found";
        throw domain_error(ss.str());
    }

    return m_rigidBodyComponentToBtRigidBodyMap.at(rigidBodyComponent);
}

void BulletPhysicsEngine::setGravity(const glm::vec3& gravity)
{
    m_dynamicsWorld->setGravity(glmVec3ToBtVector3(gravity));
}

void BulletPhysicsEngine::setPosition(RigidBodyComponent* rigidBodyComponent, const glm::vec3& position)
{
    auto rigidBody = getRigidBody(rigidBodyComponent);

    auto transform = rigidBody->getCenterOfMassTransform();
    transform.setOrigin(glmVec3ToBtVector3(position));
    rigidBody->setCenterOfMassTransform(transform);
}

void BulletPhysicsEngine::setRotation(RigidBodyComponent* rigidBodyComponent, const glm::quat& rotation)
{
}

/*void BulletPhysicsEngine::addForce(const string& rigidBodyName, const glm::vec3& force)
{
}

void BulletPhysicsEngine::addTorque(const string& rigidBodyName, const glm::vec3& torque)
{
}*/

/*void BulletPhysicsEngine::setVelocityDirectly(const string& rigidBodyName, const glm::vec3& velocity)
{
}

void BulletPhysicsEngine::setVelocityViaMotor(const string& rigidBodyName, const glm::vec3& velocity)
{
}

void BulletPhysicsEngine::setAngularVelocityDirectly(const string& rigidBodyName, const glm::vec3& angularVelocity)
{
}

void BulletPhysicsEngine::setAngularVelocityViaMotor(const string& rigidBodyName, const glm::vec3& velocity)
{
}*/

void BulletPhysicsEngine::setRigidBodyEnabled(RigidBodyComponent* rigidBodyComponent, bool isEnabled)
{
    auto rigidBody = getRigidBody(rigidBodyComponent);
    if (isEnabled) {
        if (!rigidBody->isInWorld()) {
            m_dynamicsWorld->addRigidBody(rigidBody);
        }
    } else {
        if (rigidBody->isInWorld()) {
            m_dynamicsWorld->removeRigidBody(rigidBody);
        }
    }
}

/*void BulletPhysicsEngine::createCylinderRigidBody(shared_ptr<GameObject> gameObject, string name, optional<float> massValue, float radius, float length, const glm::vec3& position, const glm::quat& rotation, const glm::vec3& maxMotorForce, const glm::vec3& maxMotorTorque)
{
}*/

void BulletPhysicsEngine::createSphereRigidBody(RigidBodyComponent* rigidBodyComponent, optional<float> massValue, float radius, const glm::vec3& position, const glm::quat& rotation)
{
    auto shape = new btSphereShape(radius);

    auto btQuaternionRotation = glmQuatToBtQuaternion(rotation);
    auto btVector3Position = glmVec3ToBtVector3(position);

    auto motionState = new btDefaultMotionState(btTransform(btQuaternionRotation, btVector3Position));

    if (massValue) {
        btVector3 bodyInertia;
        shape->calculateLocalInertia(massValue.value(), bodyInertia);
        btRigidBody::btRigidBodyConstructionInfo bodyCI = btRigidBody::btRigidBodyConstructionInfo(
            massValue.value(),
            motionState,
            shape,
            bodyInertia
        );
        bodyCI.m_restitution = 1.0f;
        bodyCI.m_friction = 0.5f;

        auto body = new btRigidBody(bodyCI);

        m_btRigidBodyToRigidBodyComponentMap.insert({ body, rigidBodyComponent });
        m_rigidBodyComponentToBtRigidBodyMap.insert({ rigidBodyComponent, body });

        m_dynamicsWorld->addRigidBody(body);
    }
    else {
        btVector3 bodyInertia;
        shape->calculateLocalInertia(0, bodyInertia);
        btRigidBody::btRigidBodyConstructionInfo bodyCI = btRigidBody::btRigidBodyConstructionInfo(
            0,
            motionState,
            shape,
            bodyInertia
        );
        bodyCI.m_restitution = 1.0f;
        bodyCI.m_friction = 0.5f;

        auto body = new btRigidBody(bodyCI);

        m_btRigidBodyToRigidBodyComponentMap.insert({ body, rigidBodyComponent });
        m_rigidBodyComponentToBtRigidBodyMap.insert({ rigidBodyComponent, body });

        m_dynamicsWorld->addRigidBody(body);
    }
}

/*void BulletPhysicsEngine::createBoxRigidBody(shared_ptr<GameObject> gameObject, string name, optional<float> massValue, const glm::vec3& size, const glm::vec3& position, const glm::quat& rotation, const glm::vec3& maxMotorForce, const glm::vec3& maxMotorTorque)
{
}

void BulletPhysicsEngine::createCharacterCapsuleRigidBody(shared_ptr<GameObject> gameObject, string name, optional<float> massValue, float radius, float length, const glm::vec3& position, const glm::vec3& maxMotorForce)
{
}

void BulletPhysicsEngine::createTriMeshRigidBody(shared_ptr<GameObject> gameObject, string name, const Mesh& mesh, optional<float> massValue, const glm::vec3& position, const glm::quat& rotation)
{
}*/

void BulletPhysicsEngine::removeRigidBody(RigidBodyComponent* rigidBodyComponent)
{
    auto rigidBody = getRigidBody(rigidBodyComponent);

    if (rigidBody->isInWorld()) {
        m_dynamicsWorld->removeRigidBody(rigidBody);
    }

    m_rigidBodyComponentToBtRigidBodyMap.erase(rigidBodyComponent);
    m_btRigidBodyToRigidBodyComponentMap.erase(rigidBody);

    delete rigidBody->getMotionState();
    delete rigidBody->getCollisionShape();
    delete rigidBody;
}

void BulletPhysicsEngine::update(float dt)
{
    m_dynamicsWorld->stepSimulation(dt);
}

void BulletPhysicsEngine::getRigidBodyRotationAndPosition(RigidBodyComponent* rigidBodyComponent, glm::mat4x4& destRotationMatrix, glm::vec3& destPosition)
{
    auto body = getRigidBody(rigidBodyComponent);
    auto transform = body->getWorldTransform();

    destPosition = btVector3ToGlmVec3(transform.getOrigin());
    destRotationMatrix = glm::mat4_cast(btQuaternionToGlmQuat(transform.getRotation()));
}

/*glm::vec3 BulletPhysicsEngine::getRigidBodyVelocity(const string& rigidBodyName)
{
    return glm::vec3();
}

void BulletPhysicsEngine::setRigidBodyFriction(const string& rigidBodyName, float friction)
{
}*/
