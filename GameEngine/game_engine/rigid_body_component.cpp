#include "rigid_body_component.h"
#include <game_engine/game_object.h>
#include <game_engine/transformation_component.h>
#include <game_engine/utils.h>
#include <game_engine/physics_engine.h>

using namespace GameEngine;
using namespace std;

RigidBodyComponent::~RigidBodyComponent() {
    auto physicsEngine = m_physicsEngine.lock();
    Utils::throwErrorIfNull(physicsEngine, []() {
        return "No Physics Engine found while destroying rigid body";
    });
    physicsEngine->removeRigidBody(this);
}

void RigidBodyComponent::update() {
    GameObjectComponent::update();

    throwErrorIfNoGameObject();
    auto gameObject = m_gameObject.lock();

    auto transform = static_pointer_cast<TransformationComponent>(
        gameObject->findComponent(TransformationComponent::TYPE_NAME)
    );
    Utils::throwErrorIfNull(transform, [&]() {
        stringstream ss;
        ss << "No transform found for game object " << gameObject->name() << " while updating rigid body";
        return ss.str();
    });

    auto physicsEngine = m_physicsEngine.lock();
    Utils::throwErrorIfNull(physicsEngine, [&]() {
        stringstream ss;
        ss << "No Physics Engine found while updating rigid body of Game Object: " << gameObject->name();
        return ss.str();
    });


    glm::mat4x4 rotationMatrix;
    glm::vec3 position;
    physicsEngine->getRigidBodyRotationAndPosition(this, rotationMatrix, position);

    glm::quat rotationQuaternion = glm::quat_cast(rotationMatrix);

    transform->setPosition(position);
    transform->setRotation(rotationQuaternion);
}

void RigidBodyComponent::setEnabled(bool isEnabled)
{
    GameObjectComponent::setEnabled(isEnabled);

    throwErrorIfNoGameObject();
    auto gameObject = m_gameObject.lock();

    auto physicsEngine = m_physicsEngine.lock();
    Utils::throwErrorIfNull(physicsEngine, [&]() {
        stringstream ss;
        if (isEnabled) {
            ss << "No Physics Engine found while enabeling Rigid Body of Game Object: " << gameObject->name();
        } else {
            ss << "No Physics Engine found while disabling Rigid Body of Game Object: " << gameObject->name();
        }
        
        return ss.str();
    });

    physicsEngine->setRigidBodyEnabled(this, isEnabled && gameObject->parent().lock() != nullptr);
}

void RigidBodyComponent::onGameObjectAttachedToParent() {
    GameObjectComponent::onGameObjectAttachedToParent();

    throwErrorIfNoGameObject();
    auto gameObject = m_gameObject.lock();

    auto transform = gameObject->findComponent<TransformationComponent>();
    Utils::throwErrorIfNull(transform, "No transform to update rigid body");
    
    auto physicsEngine = m_physicsEngine.lock();
    Utils::throwErrorIfNull(physicsEngine, [&]() {
        stringstream ss;
        ss << "No Physics Engine found while attaching rigid body to game object: " << gameObject->name();
        return ss.str();
    });

    physicsEngine->setPosition(this, transform->position());
    physicsEngine->setRotation(this, transform->rotation());
    physicsEngine->setRigidBodyEnabled(this, m_isEnabled);
}

void RigidBodyComponent::onGameObjectDetachedFromParent() {
    GameObjectComponent::onGameObjectDetachedFromParent();

    auto physicsEngine = m_physicsEngine.lock();
    Utils::throwErrorIfNull(physicsEngine, []() {
        return "No Physics Engine found while detaching rigid body";
    });
    physicsEngine->setRigidBodyEnabled(this, false);
}
