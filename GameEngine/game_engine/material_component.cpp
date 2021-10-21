#include "material_component.h"

using namespace GameEngine;
using namespace std;

const string MaterialComponent::TYPE_NAME = "MaterialComponent";

shared_ptr<GameObjectComponent> MaterialComponent::clone() {
    auto clone = make_shared<MaterialComponent>(m_material);
    clone->setEnabled(m_isEnabled);
    return clone;
}
