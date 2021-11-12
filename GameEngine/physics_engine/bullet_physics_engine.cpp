#include "bullet_physics_engine.h"
#include <game_engine/utils.h>
#include <vector>
#include <physics_engine/cylinder_rigid_body_allocated_objects.h>

using namespace GameEngine;
using namespace std;

void GameEngine::tickCallback(btDynamicsWorld* world, btScalar) {
    auto physicsEngine = reinterpret_cast<BulletPhysicsEngine*>(world->getWorldUserInfo());
    auto dispatcher = world->getDispatcher();
    auto numberOfManifolds = dispatcher->getNumManifolds();
    for (int i = 0; i < numberOfManifolds; i++) {
        auto manifold = dispatcher->getManifoldByIndexInternal(i);

        /*auto body0 = static_cast<const btRigidBody*>(manifold->getBody0());
        auto body1 = static_cast<const btRigidBody*>(manifold->getBody1());

        auto gameObject0 = physicsEngine->m_btRigidBodyToGameObjectMap.at(body0);
        auto gameObject1 = physicsEngine->m_btRigidBodyToGameObjectMap.at(body1);*/

        auto body0 = manifold->getBody0();
        auto body1 = manifold->getBody1();

        auto rigidBodyComponent0 = physicsEngine->m_btObjectsToRigidBodyComponentMap.at(const_cast<btCollisionObject*>(body0));
        auto rigidBodyComponent1 = physicsEngine->m_btObjectsToRigidBodyComponentMap.at(const_cast<btCollisionObject*>(body1));

        if (rigidBodyComponent0->isActivator() && !body1->isActive()) {
            body1->activate();
        }

        if (rigidBodyComponent1->isActivator() && !body0->isActive()) {
            body0->activate();
        }

        auto numberOfContacts = manifold->getNumContacts();
        for (int j = 0; j < numberOfContacts; j++) {
            const auto& contact = manifold->getContactPoint(j);

            /* {
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
            }*/
        }
    }
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

bool GameEngine::BulletPhysicsEngine::isKinematic(RigidBodyComponent* rigidBodyComponent)
{
    auto allocatedObjectsContainer = getBtObjects(rigidBodyComponent);

    if (holds_alternative<SphereDynamicRigidBodyAllocatedObjects*>(allocatedObjectsContainer)) {
        auto allocatedObjects = get<SphereDynamicRigidBodyAllocatedObjects*>(allocatedObjectsContainer);
        return allocatedObjects->btRigidBody->getMass() == 0;
    } else if (holds_alternative<TriMeshStaticRigidBodyAllocatedObjects*>(allocatedObjectsContainer)) {
        return true;
    } else if (holds_alternative<CylinderRigidBodyAllocatedObjects*>(allocatedObjectsContainer)) {
        auto allocatedObjects = get<CylinderRigidBodyAllocatedObjects*>(allocatedObjectsContainer);
        return allocatedObjects->btRigidBody->getMass() == 0;
    } else {
        throw domain_error("Got unsupported Allocated Objects Variant while checking if Rigid Body is Kinematic");
    }
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
    for (auto& entry : m_btObjectsToRigidBodyComponentMap) {
        rigidBodiesToRemove.push_back(entry.second);
    }

    for (auto& rigidBodyToRemove : rigidBodiesToRemove) {
        removeRigidBody(rigidBodyToRemove);
    }
}

btAllocatedObjectsContainer BulletPhysicsEngine::getBtObjects(RigidBodyComponent* rigidBodyComponent) const
{
    auto gameObject = rigidBodyComponent->gameObject().lock();
    Utils::throwErrorIfNull(gameObject, []() {
        return "Rigid Object Component has no Game Object";
    });

    if (m_rigidBodyComponentToBtObjectsMap.count(rigidBodyComponent) == 0) {
        stringstream ss;
        ss << "Bullet specific objects for Game Object " << gameObject->name() << " not found";
        throw domain_error(ss.str());
    }

    return m_rigidBodyComponentToBtObjectsMap.at(rigidBodyComponent);
}

void BulletPhysicsEngine::setGravity(const glm::vec3& gravity)
{
    m_dynamicsWorld->setGravity(glmVec3ToBtVector3(gravity));
}

void BulletPhysicsEngine::setPosition(RigidBodyComponent* rigidBodyComponent, const glm::vec3& position)
{
    auto allocatedObjectsContainer = getBtObjects(rigidBodyComponent);

    if (holds_alternative<SphereDynamicRigidBodyAllocatedObjects*>(allocatedObjectsContainer)) {
        auto allocatedObjects = get<SphereDynamicRigidBodyAllocatedObjects*>(allocatedObjectsContainer);

        auto rigidBody = allocatedObjects->btRigidBody;

        auto transform = rigidBody->getCenterOfMassTransform();
        transform.setOrigin(glmVec3ToBtVector3(position));
        rigidBody->setCenterOfMassTransform(transform);
    } else if (holds_alternative<TriMeshStaticRigidBodyAllocatedObjects*>(allocatedObjectsContainer)) {
        auto allocatedObjects = get<TriMeshStaticRigidBodyAllocatedObjects*>(allocatedObjectsContainer);

        auto collisionObject = allocatedObjects->btCollisionObject;

        auto transform = collisionObject->getWorldTransform();
        transform.setOrigin(glmVec3ToBtVector3(position));
        collisionObject->setWorldTransform(transform);
    } else if (holds_alternative<CylinderRigidBodyAllocatedObjects*>(allocatedObjectsContainer)) {
        auto allocatedObjects = get<CylinderRigidBodyAllocatedObjects*>(allocatedObjectsContainer);

        auto rigidBody = allocatedObjects->btRigidBody;

        auto transform = rigidBody->getCenterOfMassTransform();
        transform.setOrigin(glmVec3ToBtVector3(position));
        rigidBody->setCenterOfMassTransform(transform);
    }
}

void BulletPhysicsEngine::setRotation(RigidBodyComponent* rigidBodyComponent, const glm::quat& rotation)
{
    auto allocatedObjectsContainer = getBtObjects(rigidBodyComponent);

    if (holds_alternative<SphereDynamicRigidBodyAllocatedObjects*>(allocatedObjectsContainer)) {
        auto allocatedObjects = get<SphereDynamicRigidBodyAllocatedObjects*>(allocatedObjectsContainer);

        auto rigidBody = allocatedObjects->btRigidBody;

        auto transform = rigidBody->getCenterOfMassTransform();
        transform.setRotation(glmQuatToBtQuaternion(rotation));
        rigidBody->setCenterOfMassTransform(transform);
    } else if (holds_alternative<TriMeshStaticRigidBodyAllocatedObjects*>(allocatedObjectsContainer)) {
        auto allocatedObjects = get<TriMeshStaticRigidBodyAllocatedObjects*>(allocatedObjectsContainer);

        auto collisionObject = allocatedObjects->btCollisionObject;

        auto transform = collisionObject->getWorldTransform();
        transform.setRotation(glmQuatToBtQuaternion(rotation));
        collisionObject->setWorldTransform(transform);
    } else if (holds_alternative<CylinderRigidBodyAllocatedObjects*>(allocatedObjectsContainer)) {
        auto allocatedObjects = get<CylinderRigidBodyAllocatedObjects*>(allocatedObjectsContainer);

        auto rigidBody = allocatedObjects->btRigidBody;

        auto transform = rigidBody->getCenterOfMassTransform();
        transform.setRotation(glmQuatToBtQuaternion(rotation));
        rigidBody->setCenterOfMassTransform(transform);
    }
}

void BulletPhysicsEngine::addForce(RigidBodyComponent* rigidBodyComponent, const glm::vec3& force)
{
    auto allocatedObjectsContainer = getBtObjects(rigidBodyComponent);

    if (holds_alternative<SphereDynamicRigidBodyAllocatedObjects*>(allocatedObjectsContainer)) {
        auto allocatedObjects = get<SphereDynamicRigidBodyAllocatedObjects*>(allocatedObjectsContainer);

        auto rigidBody = allocatedObjects->btRigidBody;
        rigidBody->activate();
        rigidBody->applyCentralForce(glmVec3ToBtVector3(force));
    } else if (holds_alternative<TriMeshStaticRigidBodyAllocatedObjects*>(allocatedObjectsContainer)) {
        throw domain_error("Can't add Force to Static TriMesh Rigid Body");
    } else if (holds_alternative<CylinderRigidBodyAllocatedObjects*>(allocatedObjectsContainer)) {
        auto allocatedObjects = get<CylinderRigidBodyAllocatedObjects*>(allocatedObjectsContainer);

        auto rigidBody = allocatedObjects->btRigidBody;
        rigidBody->activate();
        rigidBody->applyCentralForce(glmVec3ToBtVector3(force));
    }
}

/*void BulletPhysicsEngine::addTorque(const string& rigidBodyName, const glm::vec3& torque)
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
    auto allocatedObjectsContainer = getBtObjects(rigidBodyComponent);

    if (holds_alternative<SphereDynamicRigidBodyAllocatedObjects*>(allocatedObjectsContainer)) {
        auto allocatedObjects = get<SphereDynamicRigidBodyAllocatedObjects*>(allocatedObjectsContainer);

        auto rigidBody = allocatedObjects->btRigidBody;
        if (isEnabled) {
            if (!rigidBody->isInWorld()) {
                m_dynamicsWorld->addRigidBody(rigidBody);
            }
        } else {
            if (rigidBody->isInWorld()) {
                m_dynamicsWorld->removeRigidBody(rigidBody);
            }
        }
    } else if (holds_alternative<TriMeshStaticRigidBodyAllocatedObjects*>(allocatedObjectsContainer)) {
        auto allocatedObjects = get<TriMeshStaticRigidBodyAllocatedObjects*>(allocatedObjectsContainer);

        if (!isEnabled) {
            throw domain_error("Can't disable Static TriMesh Rigid Body");
        }
    } else if (holds_alternative<CylinderRigidBodyAllocatedObjects*>(allocatedObjectsContainer)) {
        auto allocatedObjects = get<CylinderRigidBodyAllocatedObjects*>(allocatedObjectsContainer);

        auto rigidBody = allocatedObjects->btRigidBody;
        if (isEnabled) {
            if (!rigidBody->isInWorld()) {
                m_dynamicsWorld->addRigidBody(rigidBody);
            }
        }
        else {
            if (rigidBody->isInWorld()) {
                m_dynamicsWorld->removeRigidBody(rigidBody);
            }
        }
    }
}

void BulletPhysicsEngine::createCylinderRigidBody(RigidBodyComponent* rigidBodyComponent, std::optional<float> massValue, float radius, float length, const glm::vec3& position, const glm::quat& rotation)
{
    auto shape = new btCylinderShape(glmVec3ToBtVector3(glm::vec3(radius, length / 2, radius)));

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

        auto allocatedObjects = new CylinderRigidBodyAllocatedObjects();
        allocatedObjects->btMotionState = motionState;
        allocatedObjects->btCylinderShape = shape;
        allocatedObjects->btRigidBody = body;

        m_btObjectsToRigidBodyComponentMap.insert({ body, rigidBodyComponent });
        m_rigidBodyComponentToBtObjectsMap.insert({ rigidBodyComponent, allocatedObjects });

        m_dynamicsWorld->addRigidBody(body);
    } else {
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
        body->setActivationState(DISABLE_DEACTIVATION);

        auto allocatedObjects = new CylinderRigidBodyAllocatedObjects();
        allocatedObjects->btMotionState = motionState;
        allocatedObjects->btCylinderShape = shape;
        allocatedObjects->btRigidBody = body;

        m_btObjectsToRigidBodyComponentMap.insert({ body, rigidBodyComponent });
        m_rigidBodyComponentToBtObjectsMap.insert({ rigidBodyComponent, allocatedObjects });

        m_dynamicsWorld->addRigidBody(body);
    }
}

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

        auto allocatedObjects = new SphereDynamicRigidBodyAllocatedObjects();
        allocatedObjects->btMotionState = motionState;
        allocatedObjects->btSphereShape = shape;
        allocatedObjects->btRigidBody = body;

        m_btObjectsToRigidBodyComponentMap.insert({ body, rigidBodyComponent });
        m_rigidBodyComponentToBtObjectsMap.insert({ rigidBodyComponent, allocatedObjects });

        m_dynamicsWorld->addRigidBody(body);
    } else {
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
        //body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
        body->setActivationState(DISABLE_DEACTIVATION);

        auto allocatedObjects = new SphereDynamicRigidBodyAllocatedObjects();
        allocatedObjects->btMotionState = motionState;
        allocatedObjects->btSphereShape = shape;
        allocatedObjects->btRigidBody = body;

        m_btObjectsToRigidBodyComponentMap.insert({ body, rigidBodyComponent });
        m_rigidBodyComponentToBtObjectsMap.insert({ rigidBodyComponent, allocatedObjects });

        m_dynamicsWorld->addRigidBody(body);
        /*btVector3 bodyInertia;
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

        m_dynamicsWorld->addRigidBody(body);*/
    }
}

void BulletPhysicsEngine::createBoxRigidBody(RigidBodyComponent* rigidBodyComponent, std::optional<float> massValue, const glm::vec3& size, const glm::vec3& position, const glm::quat& rotation)
{
    throw domain_error("Box Rigid Bodies are not supported yet");
}

/*void BulletPhysicsEngine::createBoxRigidBody(shared_ptr<GameObject> gameObject, string name, optional<float> massValue, const glm::vec3& size, const glm::vec3& position, const glm::quat& rotation, const glm::vec3& maxMotorForce, const glm::vec3& maxMotorTorque)
{
}

void BulletPhysicsEngine::createCharacterCapsuleRigidBody(shared_ptr<GameObject> gameObject, string name, optional<float> massValue, float radius, float length, const glm::vec3& position, const glm::vec3& maxMotorForce)
{
}
*/

void BulletPhysicsEngine::createTriMeshRigidBody(RigidBodyComponent* rigidBodyComponent, const Mesh& mesh, std::optional<float> massValue, const glm::vec3& position, const glm::quat& rotation)
{
    auto btMesh = new btTriangleMesh(true, false);
    auto vertices = mesh.vertices();
    auto indices = mesh.indices();
    for (int i = 0; i < mesh.indices().size() / 3; i++) {
        btMesh->addTriangle(
            glmVec3ToBtVector3(vertices[indices[i * 3 + 0]].position()),
            glmVec3ToBtVector3(vertices[indices[i * 3 + 1]].position()),
            glmVec3ToBtVector3(vertices[indices[i * 3 + 2]].position()),
            true
        );
    }
    auto shape = new btBvhTriangleMeshShape(btMesh, true);

    auto btQuaternionRotation = glmQuatToBtQuaternion(rotation);
    auto btVector3Position = glmVec3ToBtVector3(position);


    if (massValue) {
        throw domain_error("Dynamic TriMesh Rigid Bodies are not supported");
        /*auto motionState = new btDefaultMotionState(btTransform(btQuaternionRotation, btVector3Position));

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

        m_dynamicsWorld->addRigidBody(body);*/
    } else {
        btCollisionObject* collisionObject = new btCollisionObject();
        collisionObject->setCollisionShape(shape);
        
        auto allocatedObjects = new TriMeshStaticRigidBodyAllocatedObjects();
        allocatedObjects->btMesh = btMesh;
        allocatedObjects->btCollisionShape = shape;
        allocatedObjects->btCollisionObject = collisionObject;

        m_btObjectsToRigidBodyComponentMap.insert({ collisionObject, rigidBodyComponent });
        m_rigidBodyComponentToBtObjectsMap.insert({ rigidBodyComponent, allocatedObjects });

        m_dynamicsWorld->addCollisionObject(collisionObject);
    }
}

void BulletPhysicsEngine::removeRigidBody(RigidBodyComponent* rigidBodyComponent)
{
    auto allocatedObjectsContainer = getBtObjects(rigidBodyComponent);

    if (holds_alternative<SphereDynamicRigidBodyAllocatedObjects*>(allocatedObjectsContainer)) {
        auto allocatedObjects = get<SphereDynamicRigidBodyAllocatedObjects*>(allocatedObjectsContainer);
        if (allocatedObjects->btRigidBody->isInWorld()) {
            m_dynamicsWorld->removeRigidBody(allocatedObjects->btRigidBody);
        }

        m_rigidBodyComponentToBtObjectsMap.erase(rigidBodyComponent);
        m_btObjectsToRigidBodyComponentMap.erase(allocatedObjects->btRigidBody);

        delete allocatedObjects->btMotionState;
        delete allocatedObjects->btSphereShape;
        delete allocatedObjects->btRigidBody;
        delete allocatedObjects;
    } else if (holds_alternative<TriMeshStaticRigidBodyAllocatedObjects*>(allocatedObjectsContainer)) {
        auto allocatedObjects = get<TriMeshStaticRigidBodyAllocatedObjects*>(allocatedObjectsContainer);

        m_dynamicsWorld->removeCollisionObject(allocatedObjects->btCollisionObject);

        m_rigidBodyComponentToBtObjectsMap.erase(rigidBodyComponent);
        m_btObjectsToRigidBodyComponentMap.erase(allocatedObjects->btCollisionObject);

        delete allocatedObjects->btCollisionObject;
        delete allocatedObjects->btCollisionShape;
        delete allocatedObjects->btMesh;
        delete allocatedObjects;
    } else if (holds_alternative<CylinderRigidBodyAllocatedObjects*>(allocatedObjectsContainer)) {
        auto allocatedObjects = get<CylinderRigidBodyAllocatedObjects*>(allocatedObjectsContainer);
        if (allocatedObjects->btRigidBody->isInWorld()) {
            m_dynamicsWorld->removeRigidBody(allocatedObjects->btRigidBody);
        }

        m_rigidBodyComponentToBtObjectsMap.erase(rigidBodyComponent);
        m_btObjectsToRigidBodyComponentMap.erase(allocatedObjects->btRigidBody);

        delete allocatedObjects->btMotionState;
        delete allocatedObjects->btCylinderShape;
        delete allocatedObjects->btRigidBody;
        delete allocatedObjects;
    }
}

void BulletPhysicsEngine::update(float dt)
{
    m_dynamicsWorld->stepSimulation(dt);
}

void BulletPhysicsEngine::getRigidBodyRotationAndPosition(RigidBodyComponent* rigidBodyComponent, glm::mat4x4& destRotationMatrix, glm::vec3& destPosition)
{
    auto allocatedObjectsContainer = getBtObjects(rigidBodyComponent);

    if (holds_alternative<SphereDynamicRigidBodyAllocatedObjects*>(allocatedObjectsContainer)) {
        auto allocatedObjects = get<SphereDynamicRigidBodyAllocatedObjects*>(allocatedObjectsContainer);

        auto transform = allocatedObjects->btRigidBody->getWorldTransform();

        destPosition = btVector3ToGlmVec3(transform.getOrigin());
        destRotationMatrix = glm::mat4_cast(btQuaternionToGlmQuat(transform.getRotation()));
    } else if (holds_alternative<TriMeshStaticRigidBodyAllocatedObjects*>(allocatedObjectsContainer)) {
        auto allocatedObjects = get<TriMeshStaticRigidBodyAllocatedObjects*>(allocatedObjectsContainer);

        auto transform = allocatedObjects->btCollisionObject->getWorldTransform();

        destPosition = btVector3ToGlmVec3(transform.getOrigin());
        destRotationMatrix = glm::mat4_cast(btQuaternionToGlmQuat(transform.getRotation()));
    } else if (holds_alternative<CylinderRigidBodyAllocatedObjects*>(allocatedObjectsContainer)) {
        auto allocatedObjects = get<CylinderRigidBodyAllocatedObjects*>(allocatedObjectsContainer);

        auto transform = allocatedObjects->btRigidBody->getWorldTransform();

        destPosition = btVector3ToGlmVec3(transform.getOrigin());
        destRotationMatrix = glm::mat4_cast(btQuaternionToGlmQuat(transform.getRotation()));
    } else {
        throw domain_error("Got unsupported Allocated Objects Variant while getting Rigid Body rotation and position");
    }
}

glm::vec3 BulletPhysicsEngine::getRigidBodyVelocity(RigidBodyComponent* rigidBodyComponent)
{
    auto allocatedObjectsContainer = getBtObjects(rigidBodyComponent);

    if (holds_alternative<SphereDynamicRigidBodyAllocatedObjects*>(allocatedObjectsContainer)) {
        auto allocatedObjects = get<SphereDynamicRigidBodyAllocatedObjects*>(allocatedObjectsContainer);

        return btVector3ToGlmVec3(allocatedObjects->btRigidBody->getLinearVelocity());
    } else if (holds_alternative<TriMeshStaticRigidBodyAllocatedObjects*>(allocatedObjectsContainer)) {
        throw domain_error("Can't to get velocity of Static TriMesh Rigid Body");
    } else if (holds_alternative<CylinderRigidBodyAllocatedObjects*>(allocatedObjectsContainer)) {
        auto allocatedObjects = get<CylinderRigidBodyAllocatedObjects*>(allocatedObjectsContainer);

        return btVector3ToGlmVec3(allocatedObjects->btRigidBody->getLinearVelocity());
    } else {
        throw domain_error("Got unsupported Allocated Objects Variant while getting Rigid Body velocity");
    }
}

void BulletPhysicsEngine::setRigidBodyFriction(RigidBodyComponent* rigidBodyComponent, float friction)
{
    auto allocatedObjectsContainer = getBtObjects(rigidBodyComponent);

    if (holds_alternative<SphereDynamicRigidBodyAllocatedObjects*>(allocatedObjectsContainer)) {
        auto allocatedObjects = get<SphereDynamicRigidBodyAllocatedObjects*>(allocatedObjectsContainer);

        allocatedObjects->btRigidBody->setFriction(friction);
    } else if (holds_alternative<TriMeshStaticRigidBodyAllocatedObjects*>(allocatedObjectsContainer)) {
        auto allocatedObjects = get<TriMeshStaticRigidBodyAllocatedObjects*>(allocatedObjectsContainer);

        allocatedObjects->btCollisionObject->setFriction(friction);
    } else if (holds_alternative<CylinderRigidBodyAllocatedObjects*>(allocatedObjectsContainer)) {
        auto allocatedObjects = get<CylinderRigidBodyAllocatedObjects*>(allocatedObjectsContainer);

        allocatedObjects->btRigidBody->setFriction(friction);
    }
}
