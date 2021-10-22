#pragma once

#include <game_engine/scene.h>
#include <game_engine/without_generated_methods.h>
#include <string>
#include <memory>
#include <glm/vec3.hpp>
#include <optional>
#include <game_engine/mesh.h>
#include <unordered_map>
#include <game_engine/material.h>
#include <game_engine/text_appearance.h>
#include <nlohmann/json.hpp>

namespace GameEngine
{
class ServiceLocator;

class SceneHierarchyLoader : public WithoutGeneratedMethods
{
    std::shared_ptr<ServiceLocator> m_serviceLocator;

public:
    explicit SceneHierarchyLoader(std::shared_ptr<ServiceLocator> serviceLocator) : m_serviceLocator(serviceLocator) {}

    virtual ~SceneHierarchyLoader() = default;

    virtual void loadHierarchyIntoScene(const std::string& path, Scene& scene);

private:
    void buildHierarchyFromJson(const std::string& jsonString, Scene& scene);
    float parseFloatNumber(const nlohmann::json& jsonValue);
    glm::vec3 parseColor3f(const nlohmann::json& colorJson);
    glm::vec4 parseColor4f(const nlohmann::json& colorJson);
    std::vector<std::string> parseLayerNames(const nlohmann::json& layerNamesJsonArray);
    ComplexValue parseComplexValue(const nlohmann::json& jsonValue, std::optional<DimensionType> optionalDimensionType);
    shared_ptr<GameObjectComponent> parseComponent(
        const shared_ptr<GameObject>& gameObject,
        const nlohmann::json& componentJson,
        const unordered_map<string, Material>& materialsMap,
        const unordered_map<string, TextAppearance>& textAppearancesMap,
        const MeshStorage& meshStorage
    );
    Mesh createTransformedMesh(const Mesh& mesh, const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale);
};
}
