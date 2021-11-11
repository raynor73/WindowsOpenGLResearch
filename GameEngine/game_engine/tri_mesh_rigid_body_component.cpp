#include "tri_mesh_rigid_body_component.h"
#include <game_engine/physics_engine.h>

using namespace GameEngine;
using namespace std;

const string TriMeshRigidBodyComponent::TYPE_NAME = "TriMeshRigidBodyComponent";

TriMeshRigidBodyComponent::TriMeshRigidBodyComponent(
    weak_ptr<PhysicsEngine> physicsEngine, 
    optional<float> massValue, 
    const Mesh& mesh
) : RigidBodyComponent(physicsEngine), m_massValue(massValue), m_mesh(mesh)
{
    auto physicsEngineStrongPtr = physicsEngine.lock();
    if (physicsEngineStrongPtr == nullptr) {
        throw domain_error("No Physics Engine while creating TriMesh Rigid Body");
    }

    physicsEngineStrongPtr->createTriMeshRigidBody(this, m_mesh, m_massValue, glm::vec3(0), glm::identity<glm::quat>());
}

shared_ptr<GameObjectComponent> TriMeshRigidBodyComponent::clone()
{
    auto physicsEngine = m_physicsEngine.lock();
    if (physicsEngine == nullptr) {
        throw domain_error("No Physics Engine while cloning TriMesh Rigid Body");
    }

    auto clone = std::make_shared<TriMeshRigidBodyComponent>(
        physicsEngine,
        m_massValue,
        m_mesh
    );
    clone->setEnabled(m_isEnabled);
    return clone;
}
