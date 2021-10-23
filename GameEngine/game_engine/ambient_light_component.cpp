#include "ambient_light_component.h"

using namespace GameEngine;
using namespace std;

const string AmbientLightComponent::TYPE_NAME = "AmbientLightComponent";

shared_ptr<GameObjectComponent> AmbientLightComponent::clone() {
    auto clone = make_shared<AmbientLightComponent>(m_color, m_layerNames);
    clone->setEnabled(m_isEnabled);
    return clone;
}