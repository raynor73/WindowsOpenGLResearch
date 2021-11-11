#include "cylinder_rigid_body_component.h"
#include <game_engine/physics_engine.h>

using namespace GameEngine;
using namespace std;

const string CylinderRigidBodyComponent::TYPE_NAME = "CylinderRigidBodyComponent";

CylinderRigidBodyComponent::CylinderRigidBodyComponent(
    std::weak_ptr<PhysicsEngine> physicsEngine,
    optional<float> massValue,
    float radius,
    float length
) : RigidBodyComponent(physicsEngine), m_massValue(massValue), m_radius(radius), m_length(length) {
    auto physicsEngineStrongPtr = physicsEngine.lock();
    if (physicsEngineStrongPtr == nullptr) {
        throw domain_error("No Physics Engine while creating Cylinder Rigid Body");
    }

    physicsEngineStrongPtr->createCylinderRigidBody(this, m_massValue, m_radius, m_length, glm::vec3(0), glm::identity<glm::quat>());
}

shared_ptr<GameObjectComponent> CylinderRigidBodyComponent::clone() {
    auto physicsEngine = m_physicsEngine.lock();
    if (physicsEngine == nullptr) {
        throw domain_error("No Physics Engine while cloning Cylinder Rigid Body");
    }

    auto clone = std::make_shared<CylinderRigidBodyComponent>(
        physicsEngine,
        m_massValue,
        m_radius,
        m_length
    );
    clone->setEnabled(m_isEnabled);
    return clone;
}
