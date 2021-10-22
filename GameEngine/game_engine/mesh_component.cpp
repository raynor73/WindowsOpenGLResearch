#include "mesh_component.h"

using namespace GameEngine;
using namespace std;

const string MeshComponent::TYPE_NAME = "MeshComponent";

shared_ptr<GameObjectComponent> MeshComponent::clone() {
    auto clone = make_shared<MeshComponent>(m_mesh, m_meshName);
    clone->setEnabled(m_isEnabled);
    return clone;
}
