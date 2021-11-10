#include "sphere_rigid_body_component.h"
#include <game_engine/physics_engine.h>

using namespace GameEngine;
using namespace std;

const string SphereRigidBodyComponent::TYPE_NAME = "SphereRigidBodyComponent";

SphereRigidBodyComponent::SphereRigidBodyComponent(
    std::shared_ptr<PhysicsEngine> physicsEngine,
    optional<float> massValue,
    float radius
) : RigidBodyComponent(physicsEngine), m_massValue(massValue), m_radius(radius) {
    if (physicsEngine == nullptr) {
        throw domain_error("No Physics Engine while creating Sphere Rigid Body");
    }

    physicsEngine->createSphereRigidBody(this, m_massValue, m_radius, glm::vec3(0), glm::identity<glm::quat>());
}

shared_ptr<GameObjectComponent> SphereRigidBodyComponent::clone() {
    auto physicsEngine = m_physicsEngine.lock();
    if (physicsEngine == nullptr) {
        throw domain_error("No Physics Engine while cloning Sphere Rigid Body");
    }

    auto clone = std::make_shared<SphereRigidBodyComponent>(
        physicsEngine,
        m_massValue,
        m_radius
    );
    clone->setEnabled(m_isEnabled);
    return clone;
}
