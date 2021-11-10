#pragma once

#include <memory>
#include <unordered_map>
#include <string>
#include <game_engine/ambient_light_component.h>
#include <game_engine/directional_light_component.h>
#include <game_engine/without_generated_methods.h>

namespace GameEngine
{
class LightComponentsManager : public WithoutGeneratedMethods
{
    std::unordered_map<std::string, std::shared_ptr<AmbientLightComponent>> m_layerNameToAmbientLightMap;
    std::unordered_multimap<std::string, std::shared_ptr<DirectionalLightComponent>> m_layerNameToDirectionalLightsMap;

    std::unordered_multimap<std::shared_ptr<AmbientLightComponent>, std::string> m_ambientLightToLayerNamesMap;
    std::unordered_multimap<std::shared_ptr<DirectionalLightComponent>, std::string> m_directionalLightToLayerNamesMap;
public:
    const std::unordered_map<std::string, std::shared_ptr<AmbientLightComponent>>& layerNameToAmbientLightMap() { return m_layerNameToAmbientLightMap; }
    const std::unordered_multimap<std::string, std::shared_ptr<DirectionalLightComponent>>& layerNameToDirectionalLightsMap() { return m_layerNameToDirectionalLightsMap; }

    std::shared_ptr<AmbientLightComponent> createAmbientLight(glm::vec3 color, std::vector<std::string> layerNames);
    std::shared_ptr<DirectionalLightComponent> createDirectionalLight(glm::vec3 color, glm::vec3 direction, std::vector<std::string> layerNames);

    void releaseAmbientLight(std::shared_ptr<AmbientLightComponent> ambientLight);
    void releaseDirectionalLight(std::shared_ptr<DirectionalLightComponent> directionalLight);

    void reset();
};
}
