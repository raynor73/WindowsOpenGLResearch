#include "light_components_manager.h"

using namespace GameEngine;
using namespace std;

shared_ptr<AmbientLightComponent> LightComponentsManager::createAmbientLight(glm::vec3 color, vector<string> layerNames)
{
    auto ambientLight = new AmbientLightComponent(color, layerNames);
    auto ambientLightSharedPtr = shared_ptr<AmbientLightComponent>(ambientLight);

    for (auto layerName : layerNames) {
        m_layerNameToAmbientLightMap.insert({ layerName, ambientLightSharedPtr });
        m_ambientLightToLayerNamesMap.insert({ ambientLightSharedPtr, layerName });
    }

    return ambientLightSharedPtr;
}

shared_ptr<DirectionalLightComponent> LightComponentsManager::createDirectionalLight(glm::vec3 color, glm::vec3 direction, std::vector<std::string> layerNames)
{
    auto directironalLight = shared_ptr<DirectionalLightComponent>(new DirectionalLightComponent(color, direction, layerNames));

    for (auto layerName : layerNames) {
        m_layerNameToDirectionalLightsMap.insert({ layerName, directironalLight });
        m_directionalLightToLayerNamesMap.insert({ directironalLight, layerName });
    }

    return directironalLight;
}

void LightComponentsManager::releaseAmbientLight(shared_ptr<AmbientLightComponent> ambientLight)
{
    auto layerNamesRange = m_ambientLightToLayerNamesMap.equal_range(ambientLight);

    for (auto layerNamesIterator = layerNamesRange.first; layerNamesIterator != layerNamesRange.second; ++layerNamesIterator) {
        m_layerNameToAmbientLightMap.erase(layerNamesIterator->second);

    }
    m_ambientLightToLayerNamesMap.erase(ambientLight);
}

void LightComponentsManager::releaseDirectionalLight(shared_ptr<DirectionalLightComponent> directionalLight)
{
    auto layerNamesRange = m_directionalLightToLayerNamesMap.equal_range(directionalLight);

    for (auto layerNamesIterator = layerNamesRange.first; layerNamesIterator != layerNamesRange.second; ++layerNamesIterator) {
        auto directionalLightsRange = m_layerNameToDirectionalLightsMap.equal_range(layerNamesIterator->second);
        for (auto directionalLightsIterator = directionalLightsRange.first; directionalLightsIterator != directionalLightsRange.second; ++directionalLightsIterator) {
            if (directionalLightsIterator->second == directionalLight) {
                m_layerNameToDirectionalLightsMap.erase(directionalLightsIterator);
                break;
            }
        }
    }
    m_directionalLightToLayerNamesMap.erase(directionalLight);
}
