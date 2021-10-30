#include "directional_light_component.h"

using namespace GameEngine;
using namespace std;

const std::string DirectionalLightComponent::TYPE_NAME = "DirectionalLightComponent";

std::shared_ptr<GameObjectComponent> DirectionalLightComponent::clone() {
    auto clone = new DirectionalLightComponent(
        m_color,
        m_direction,
        m_layerNames
    );
    clone->setEnabled(m_isEnabled);
    return shared_ptr<DirectionalLightComponent>(clone);
}
