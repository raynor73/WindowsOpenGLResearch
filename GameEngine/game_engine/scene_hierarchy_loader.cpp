#include "scene_hierarchy_loader.h"
#include <game_engine/service_locator.h>
#include <glm/vec3.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <game_engine/mesh_component.h>
#include <game_engine/material_component.h>
#include <game_engine/ortho_camera_component.h>
#include <game_engine/perspective_camera_component.h>
#include <game_engine/logger.h>
#include <game_engine/constants.h>
#include <game_engine/transformation_component.h>
#include <game_engine/game_object.h>
#include <game_engine/ambient_light_component.h>
#include <game_engine/directional_light_component.h>
#include <game_engine/sphere_rigid_body_component.h>
#include <game_engine/tri_mesh_rigid_body_component.h>
#include <game_engine/cylinder_rigid_body_component.h>

using namespace GameEngine;
using namespace std;

void SceneHierarchyLoader::loadHierarchyIntoScene(const string& path, Scene& scene)
{
    buildHierarchyFromJson(m_serviceLocator->readOnlyFsAbstraction()->readTextFileContent(path), scene);
}

void SceneHierarchyLoader::buildHierarchyFromJson(const string& jsonString, Scene& scene)
{
    unordered_map<string, Material> materialsMap;
    unordered_map<string, TextAppearance> textAppearancesMap;

    nlohmann::json sceneJson;
    try {
        sceneJson = nlohmann::json::parse(jsonString);
    } catch (nlohmann::json::parse_error& e) {
        L::e(Constants::LOG_TAG, "Error restoring scene", e);
    }

    auto physicsParamsJson = sceneJson["physicsParams"];
    auto gravityJson = physicsParamsJson["gravity"];
    m_serviceLocator->physicsEngine().lock()->setGravity(glm::vec3(
        parseFloatNumber(gravityJson[0]),
        parseFloatNumber(gravityJson[1]),
        parseFloatNumber(gravityJson[2])
    ));

    auto meshesJsonArray = sceneJson["meshes"];
    if (meshesJsonArray.is_array()) {
        for (auto& meshJson : meshesJsonArray) {
            string name;
            string path;

            if (!meshJson["name"].is_null()) {
                name = meshJson["name"].get<string>();
            } else {
                continue;
            }

            if (!meshJson["path"].is_null()) {
                path = meshJson["path"].get<string>();
            } else {
                continue;
            }

            scene.meshStorage().putMesh(name, m_serviceLocator->meshLoader()->loadMesh(path));
        }
    }

    /*auto texturesJsonArray = sceneJson["textures"];
    if (texturesJsonArray.is_array()) {
        for (auto& textureJson : texturesJsonArray) {
            auto nameJson = textureJson["name"];
            if (!nameJson.is_string()) {
                continue;
            }
            auto pathJson = textureJson["path"];
            if (!pathJson.is_string()) {
                continue;
            }
            auto displayDensityFactorAwareJson = textureJson["displayDensityFactorAware"];
            bool displayDensityFactorAware =
                displayDensityFactorAwareJson.is_boolean() &&
                displayDensityFactorAwareJson.get<bool>();
            auto name = nameJson.get<string>();
            if (displayDensityFactorAware) {
                m_texturesRepository->createDisplayDensityFactorAwareTexture(
                    name, pathJson.get<string>()
                );
            }
            else {
                m_texturesRepository->createTexture(name, pathJson.get<string>());
            }
        }
    }*/

    auto materialsJsonArray = sceneJson["materials"];
    if (materialsJsonArray.is_array()) {
        for (auto& materialJson : materialsJsonArray) {
            auto nameJson = materialJson["name"];
            if (nameJson.is_null()) {
                continue;
            }
            auto isTranslucent =
                materialJson.contains("isTranslucent") &&
                materialJson["isTranslucent"].is_boolean() &&
                materialJson["isTranslucent"].get<bool>();
            auto isWireframe =
                materialJson.contains("isWireframe") &&
                materialJson["isWireframe"].is_boolean() &&
                materialJson["isWireframe"].get<bool>();
            auto isUnlit =
                materialJson.contains("isUnlit") &&
                materialJson["isUnlit"].is_boolean() &&
                materialJson["isUnlit"].get<bool>();
            auto diffuseColorJson = materialJson["diffuseColor"];
            if (diffuseColorJson.is_array()) {
                Material material{
                    parseColor4f(materialJson["diffuseColor"]),
                    materialJson.contains("topColor") ? parseColor4f(materialJson["topColor"]) : glm::vec4(0),
                    materialJson.contains("bottomColor") ? parseColor4f(materialJson["bottomColor"]) : glm::vec4(0),
                    "",
                    true,
                    isTranslucent,
                    isWireframe,
                    isUnlit,
                    materialJson.contains("isGradient") ? materialJson["isGradient"].get<bool>() : false,
                    materialJson.contains("isDoubleSided") ? materialJson["isDoubleSided"].get<bool>() : false
                };
                materialsMap[nameJson.get<string>()] = material;
            } else {
                auto textureNameJson = materialJson["textureName"];
                if (!textureNameJson.is_string()) {
                    continue;
                }
                Material material{
                    glm::vec4(0),
                    materialJson.contains("topColor") ? parseColor4f(materialJson["topColor"]) : glm::vec4(0),
                    materialJson.contains("bottomColor") ? parseColor4f(materialJson["bottomColor"]) : glm::vec4(0),
                    textureNameJson.get<string>(),
                    false,
                    isTranslucent,
                    isWireframe,
                    isUnlit,
                    materialJson.contains("isGradient") ? materialJson["isGradient"].get<bool>() : false,
                    materialJson.contains("isDoubleSided") ? materialJson["isDoubleSided"].get<bool>() : false
                };
                materialsMap[nameJson.get<string>()] = material;
            }
        }
    }

    auto textAppearancesJsonArray = sceneJson["textAppearances"];
    if (textAppearancesJsonArray.is_array()) {
        for (auto& textAppearanceJson : textAppearancesJsonArray) {
            auto nameJson = textAppearanceJson["name"];
            if (!nameJson.is_string()) {
                continue;
            }
            auto textSize = parseComplexValue(textAppearanceJson["textSize"]);
            auto fontPathJson = textAppearanceJson["fontPath"];
            if (!fontPathJson.is_string()) {
                continue;
            }
            TextAppearance textAppearance{ textSize, fontPathJson.get<string>() };
            textAppearancesMap.insert({ nameJson.get<string>(), textAppearance });
        }
    }

    /*if (sceneJson.contains("skeletalAnimations")) {
        auto skeletalAnimationsJsonArray = sceneJson["skeletalAnimations"];
        if (skeletalAnimationsJsonArray.is_array()) {
            for (auto& skeletalAnimationJson : skeletalAnimationsJsonArray) {
                auto meshName = skeletalAnimationJson["meshName"].get<string>();
                auto animatedMesh = m_meshStorage.getMesh(meshName);
                auto skeletalAnimation = m_skeletalAnimationLoadingRepository->loadAnimation(
                    animatedMesh,
                    skeletalAnimationJson["path"].get<string>()
                );
                m_meshStorage.removeMesh(meshName);
                m_meshStorage.putMesh(meshName, animatedMesh);
                m_skeletalAnimationStorage.putAnimation(
                    skeletalAnimationJson["name"].get<string>(),
                    skeletalAnimation
                );
            }
        }
    }*/

    /*if (sceneJson.contains("sounds")) {
        auto soundsJsonArray = sceneJson["sounds"];
        if (soundsJsonArray.is_array()) {
            for (auto& soundJson : soundsJsonArray) {
                m_soundStorage->putSound(
                    soundJson["name"].get<string>(),
                    m_soundLoadingRepository->loadSound(soundJson["path"])
                );
            }
        }
    }*/

    auto gameObjectsJsonArray = sceneJson["gameObjects"];
    if (gameObjectsJsonArray.is_array()) {
        for (auto& gameObjectJson : gameObjectsJsonArray) {
            string name;
            string parentName;
            glm::vec3 position;
            glm::quat rotation;
            glm::vec3 scale;

            if (gameObjectJson["name"].is_string()) {
                name = gameObjectJson["name"].get<string>();
            } else {
                continue;
            }

            if (gameObjectJson["parent"].is_string()) {
                parentName = gameObjectJson["parent"].get<string>();
            }

            if (gameObjectJson["position"].is_array() && gameObjectJson["position"].size() == 3) {
                try {
                    position.x = parseFloatNumber(gameObjectJson["position"][0]);
                    position.y = parseFloatNumber(gameObjectJson["position"][1]);
                    position.z = parseFloatNumber(gameObjectJson["position"][2]);
                } catch (exception& e) {
                    L::e(Constants::LOG_TAG, "Error parsing position values", e);
                    continue;
                }
            } else {
                continue;
            }

            if (gameObjectJson["rotation"].is_array() && gameObjectJson["rotation"].size() == 3) {
                try {
                    rotation.x = parseFloatNumber(gameObjectJson["rotation"][0]);
                    rotation.y = parseFloatNumber(gameObjectJson["rotation"][1]);
                    rotation.z = parseFloatNumber(gameObjectJson["rotation"][2]);
                } catch (exception& e) {
                    L::e(Constants::LOG_TAG, "Error parsing rotation values", e);
                    continue;
                }
            } else {
                continue;
            }

            if (gameObjectJson["scale"].is_array() && gameObjectJson["scale"].size() == 3) {
                try {
                    scale.x = parseFloatNumber(gameObjectJson["scale"][0]);
                    scale.y = parseFloatNumber(gameObjectJson["scale"][1]);
                    scale.z = parseFloatNumber(gameObjectJson["scale"][2]);
                } catch (exception& e) {
                    L::e(Constants::LOG_TAG, "Error parsing scale values", e);
                    continue;
                }
            } else {
                continue;
            }

            auto rotationMatrix = glm::identity<glm::mat4>();
            rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.z), glm::vec3(0, 0, 1));
            rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.x), glm::vec3(1, 0, 0));
            rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.y), glm::vec3(0, 1, 0));
            auto rotationQuaternion = glm::quat_cast(rotationMatrix);

            shared_ptr<GameObject> gameObject;
            if (name != "root") {
                gameObject = GameObject::create(name);
                scene.addGameObject(parentName, gameObject);
            } else {
                gameObject = scene.rootGameObject();
            }
            auto transform = make_shared<TransformationComponent>(
                position,
                rotationQuaternion,
                scale
                );
            gameObject->addComponent(transform);

            auto componentsJsonArray = gameObjectJson["components"];
            if (componentsJsonArray.is_array()) {
                for (auto& componentJson : componentsJsonArray) {
                    auto component = parseComponent(
                        gameObject,
                        componentJson,
                        materialsMap,
                        textAppearancesMap,
                        scene.meshStorage()
                    );
                    gameObject->addComponent(component);
                }
            }
        }
    }
}

float SceneHierarchyLoader::parseFloatNumber(const nlohmann::json& jsonValue)
{
    if (!jsonValue.is_number()) {
        throw domain_error("Float number has invalid format or missing");
    }
    return jsonValue.get<float>();
}

glm::vec3 SceneHierarchyLoader::parseColor3f(const nlohmann::json& colorJson)
{
    if (!colorJson.is_array() || colorJson.size() != 3) {
        throw domain_error("Invalid or missing 3 components color");
    }

    if (!colorJson[0].is_number()) {
        throw domain_error("Invalid red component of 3 components color");
    }
    auto red = colorJson[0].get<float>();

    if (!colorJson[1].is_number()) {
        throw domain_error("Invalid green component of 3 components color");
    }
    auto green = colorJson[1].get<float>();

    if (!colorJson[2].is_number()) {
        throw domain_error("Invalid blue component of 3 components color");
    }
    auto blue = colorJson[2].get<float>();

    return glm::vec3{ red, green, blue };
}

glm::vec4 SceneHierarchyLoader::parseColor4f(const nlohmann::json& colorJson)
{
    if (!colorJson.is_array() || colorJson.size() != 4) {
        throw domain_error("Invalid or missing 4 components color");
    }

    if (!colorJson[0].is_number()) {
        throw domain_error("Invalid red component of 4 components color");
    }
    auto red = colorJson[0].get<float>();

    if (!colorJson[1].is_number()) {
        throw domain_error("Invalid green component of 4 components color");
    }
    auto green = colorJson[1].get<float>();

    if (!colorJson[2].is_number()) {
        throw domain_error("Invalid blue component of 4 components color");
    }
    auto blue = colorJson[2].get<float>();

    if (!colorJson[3].is_number()) {
        throw domain_error("Invalid alpha component of 4 components color");
    }
    auto alpha = colorJson[3].get<float>();

    return glm::vec4{ red, green, blue, alpha };
}

vector<string> SceneHierarchyLoader::parseLayerNames(const nlohmann::json& layerNamesJsonArray)
{
    if (layerNamesJsonArray.is_null() || !layerNamesJsonArray.is_array()) {
        throw domain_error("Bad layer names JSON");
    }
    vector<string> layerNames;
    for (auto& layerNameJson : layerNamesJsonArray) {
        if (!layerNameJson.is_string()) {
            throw domain_error("String expected as layer name");
        }
        layerNames.push_back(layerNameJson.get<string>());
    }
    return layerNames;
}

ComplexValue SceneHierarchyLoader::parseComplexValue(const nlohmann::json& jsonValue, optional<DimensionType> optionalDimensionType)
{
    if (jsonValue.is_number()) {
        return PlainValue{ jsonValue.get<float>() };
    }
    else if (jsonValue.is_string()) {
        auto stringValue = jsonValue.get<string>();
        if (stringValue[stringValue.size() - 1] == '%') {
            auto value = stof(stringValue.substr(0, stringValue.size() - 1));
            if (!optionalDimensionType) {
                throw domain_error("Dimension type not provided for percent complex value");
            }
            return PercentValue{ optionalDimensionType.value(), value };
        }
        else if (stringValue.substr(stringValue.size() - 2, 2) == "dp") {
            return DpValue{ stof(stringValue.substr(0, stringValue.size() - 2)) };
        }
        else {
            throw domain_error("Bad number format");
        }
    }
    else {
        throw domain_error("JSON value is not a number or string");
    }
}

Mesh SceneHierarchyLoader::createTransformedMesh(const Mesh& mesh, const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale)
{
    vector<Vertex> transformedVertices;
    auto transformMatrix = glm::translate(glm::identity<glm::mat4>(), position);
    transformMatrix *= glm::toMat4(rotation);
    transformMatrix = glm::scale(transformMatrix, scale);

    for (auto& vertex : mesh.vertices()) {
        auto transformedPosition = transformMatrix * glm::vec4(vertex.position(), 1);
        auto transformedNormal = glm::normalize(transformMatrix * glm::vec4(vertex.normal(), 0));
        transformedVertices.emplace_back(
            Vertex{
                transformedPosition,
                transformedNormal,
                vertex.uv(),
                vertex.jointIndices(),
                vertex.jointWeights()
            }
        );
    }

    return Mesh(transformedVertices, mesh.indices());
}

shared_ptr<GameObjectComponent> SceneHierarchyLoader::parseComponent(
    const shared_ptr<GameObject>& gameObject,
    const nlohmann::json& componentJson,
    const unordered_map<string, Material>& materialsMap,
    const unordered_map<string, TextAppearance>& textAppearancesMap,
    const MeshStorage& meshStorage
) {
    if (!componentJson.contains("type")) {
        throw domain_error("No type found while parsing component");
    }
    auto type = componentJson["type"].get<string>();
    if (type == "Mesh") {
        auto meshNameJson = componentJson["meshName"];
        if (!meshNameJson.is_string()) {
            throw domain_error("No mesh name provided");
        }
        auto meshName = meshNameJson.get<string>();
        return make_shared<MeshComponent>(meshStorage.getMesh(meshName), meshName);
    } else if (type == "Material") {
        if (!componentJson["materialName"].is_string()) {
            throw domain_error("No material name provided");
        }
        auto materialName = componentJson["materialName"].get<string>();
        if (materialsMap.count(materialName) == 0) {
            stringstream ss;
            ss << "Material " << materialName << " not found";
            throw domain_error(ss.str());
        }
        return make_shared<MaterialComponent>(materialsMap.at(materialName));
    }
    else if (type == "MeshRenderer") {
        return m_serviceLocator->meshRendererFactory()->createMeshRenderer(parseLayerNames(componentJson["layerNames"]));
    }
    else if (type == "OrthoCamera") {
        auto camera = m_serviceLocator->cameraComponentsManager()->createOrthoCamera(
            m_serviceLocator,
            parseColor4f(componentJson["clearColor"]),
            parseLayerNames(componentJson["layerNames"]),
            parseComplexValue(componentJson["left"], DimensionType::WIDTH),
            parseComplexValue(componentJson["top"], DimensionType::HEIGHT),
            parseComplexValue(componentJson["right"], DimensionType::WIDTH),
            parseComplexValue(componentJson["bottom"], DimensionType::HEIGHT),
            parseFloatNumber(componentJson["zNear"]),
            parseFloatNumber(componentJson["zFar"]),
            componentJson["order"].get<int>()
        );
        camera->setShouldClearColor(
            componentJson.contains("shouldClearColor") &&
            componentJson["shouldClearColor"].is_boolean() &&
            componentJson["shouldClearColor"].get<bool>()
        );
        camera->setShouldClearDepth(
            componentJson.contains("shouldClearDepth") &&
            componentJson["shouldClearDepth"].is_boolean() &&
            componentJson["shouldClearDepth"].get<bool>()
        );
        return camera;
    }
    else if (type == "AmbientLight") {
        return m_serviceLocator->lightComponentsManager()->createAmbientLight(
            parseColor3f(componentJson["color"]),
            parseLayerNames(componentJson["layerNames"])
        );
    }
    /*else if (type == "Text") {
        auto textAppearanceNameJson = componentJson["textAppearanceName"];
        if (!textAppearanceNameJson.is_string()) {
            throw domain_error("No Text Appearance Name provided for Text Component");
        }
        auto textJson = componentJson["text"];
        if (!textJson.is_string()) {
            throw domain_error("No Text provided for Text Component");
        }

        auto textAppearanceName = textAppearanceNameJson.get<string>();
        if (textAppearancesMap.count(textAppearanceName) == 0) {
            stringstream ss;
            ss << "Can't find Text Appearance " << textAppearanceName;
            throw domain_error(ss.str());
        }
        return make_shared<TextComponent>(
            textJson.get<string>(),
            parseColor4f(componentJson["textColor"]),
            textAppearancesMap.at(textAppearanceName)
            );
    }
    else if (type == "TextRenderer") {
        return m_textRendererFactory->createTextRenderer(parseLayerNames(componentJson["layerNames"]));
    }
    else if (type == "ViewBounds") {
        auto left = parseComplexValue(componentJson["left"], DimensionType::WIDTH);
        auto bottom = parseComplexValue(componentJson["bottom"], DimensionType::HEIGHT);

        if (componentJson.contains("right") && componentJson.contains("top")) {
            auto right = parseComplexValue(componentJson["right"], DimensionType::WIDTH);
            auto top = parseComplexValue(componentJson["top"], DimensionType::HEIGHT);
            return make_shared<ViewBoundsComponent>(
                m_displayInfo,
                m_unitsConverter,
                EdgesInitParams{ left, top, right, bottom }
            );
        }
        else if (componentJson.contains("width") && componentJson.contains("height")) {
            auto width = parseComplexValue(componentJson["width"], DimensionType::WIDTH);
            auto height = parseComplexValue(componentJson["height"], DimensionType::HEIGHT);
            return make_shared<ViewBoundsComponent>(
                m_displayInfo,
                m_unitsConverter,
                SizeInitParams{ left, bottom, width, height }
            );
        }
        else {
            throw domain_error("Insufficient params set for view bounds");
        }
    }
    else if (type == "Layout") {
        auto paddingJson = componentJson["padding"];

        auto paddingLeft = parseComplexValue(paddingJson[0]);
        auto paddingTop = parseComplexValue(paddingJson[1]);
        auto paddingRight = parseComplexValue(paddingJson[2]);
        auto paddingBottom = parseComplexValue(paddingJson[3]);

        auto verticalLayoutJson = componentJson["verticalLayout"];
        if (!verticalLayoutJson.is_string()) {
            throw domain_error("Vertical layout parameter not found");
        }
        auto horizontalLayoutJson = componentJson["horizontalLayout"];
        if (!horizontalLayoutJson.is_string()) {
            throw domain_error("Horizontal layout parameter not found");
        }
        auto originVerticalLayoutJson = componentJson["originVerticalLayout"];
        if (!originVerticalLayoutJson.is_string()) {
            throw domain_error("Origin vertical layout parameter not found");
        }
        auto originHorizontalLayoutJson = componentJson["originHorizontalLayout"];
        if (!originHorizontalLayoutJson.is_string()) {
            throw domain_error("Origin horizontal layout parameter not found");
        }

        auto referenceViewBoundsNameJson = componentJson["referenceViewBoundsName"];
        if (!referenceViewBoundsNameJson.is_string()) {
            throw domain_error("No reference view bounds name found");
        }
        auto referenceGameObjectName = referenceViewBoundsNameJson.get<string>();
        auto referenceGameObject = m_gameObjectsMap.at(referenceGameObjectName);
        if (referenceGameObject == nullptr) {
            stringstream ss;
            ss << "Reference game object " << referenceGameObjectName << " not found";
            throw domain_error(ss.str());
        }
        auto referenceViewBounds = static_pointer_cast<ViewBoundsComponent>(
            referenceGameObject->findComponent(ViewBoundsComponent::TYPE_NAME)
            );
        if (referenceViewBounds == nullptr) {
            stringstream ss;
            ss << "Reference view bounds " << referenceGameObjectName << " not found";
            throw domain_error(ss.str());
        }

        auto verticalLayoutString = verticalLayoutJson.get<string>();
        auto horizontalLayoutString = horizontalLayoutJson.get<string>();
        auto originVerticalLayoutString = originVerticalLayoutJson.get<string>();
        auto originHorizontalLayoutString = originHorizontalLayoutJson.get<string>();

        VerticalLayoutType verticalLayoutType;
        if (verticalLayoutString == "TOP_INSIDE") {
            verticalLayoutType = VerticalLayoutType::TOP_INSIDE;
        }
        else if (verticalLayoutString == "TOP_OUTSIDE") {
            verticalLayoutType = VerticalLayoutType::TOP_OUTSIDE;
        }
        else if (verticalLayoutString == "CENTER") {
            verticalLayoutType = VerticalLayoutType::CENTER;
        }
        else if (verticalLayoutString == "BOTTOM_INSIDE") {
            verticalLayoutType = VerticalLayoutType::BOTTOM_INSIDE;
        }
        else if (verticalLayoutString == "BOTTOM_OUTSIDE") {
            verticalLayoutType = VerticalLayoutType::BOTTOM_OUTSIDE;
        }
        else {
            stringstream ss;
            ss << "Unknown vertical layout " << verticalLayoutString;
            throw domain_error(ss.str());
        }

        HorizontalLayoutType horizontalLayoutType;
        if (horizontalLayoutString == "LEFT_INSIDE") {
            horizontalLayoutType = HorizontalLayoutType::LEFT_INSIDE;
        }
        else if (horizontalLayoutString == "LEFT_OUTSIDE") {
            horizontalLayoutType = HorizontalLayoutType::LEFT_OUTSIDE;
        }
        else if (horizontalLayoutString == "CENTER") {
            horizontalLayoutType = HorizontalLayoutType::CENTER;
        }
        else if (horizontalLayoutString == "RIGHT_INSIDE") {
            horizontalLayoutType = HorizontalLayoutType::RIGHT_INSIDE;
        }
        else if (horizontalLayoutString == "RIGHT_OUTSIDE") {
            horizontalLayoutType = HorizontalLayoutType::RIGHT_OUTSIDE;
        }
        else {
            stringstream ss;
            ss << "Unknown horizontal layout " << verticalLayoutString;
            throw domain_error(ss.str());
        }

        OriginVerticalLayoutType originVerticalLayoutType;
        if (originVerticalLayoutString == "TOP") {
            originVerticalLayoutType = OriginVerticalLayoutType::TOP;
        }
        else if (originVerticalLayoutString == "CENTER") {
            originVerticalLayoutType = OriginVerticalLayoutType::CENTER;
        }
        else if (originVerticalLayoutString == "BOTTOM") {
            originVerticalLayoutType = OriginVerticalLayoutType::BOTTOM;
        }
        else {
            stringstream ss;
            ss << "Unknown origin vertical layout " << verticalLayoutString;
            throw domain_error(ss.str());
        }

        OriginHorizontalLayoutType originHorizontalLayoutType;
        if (originHorizontalLayoutString == "LEFT") {
            originHorizontalLayoutType = OriginHorizontalLayoutType::LEFT;
        }
        else if (originHorizontalLayoutString == "CENTER") {
            originHorizontalLayoutType = OriginHorizontalLayoutType::CENTER;
        }
        else if (originHorizontalLayoutString == "RIGHT") {
            originHorizontalLayoutType = OriginHorizontalLayoutType::RIGHT;
        }
        else {
            stringstream ss;
            ss << "Unknown origin horizontal layout " << verticalLayoutString;
            throw domain_error(ss.str());
        }

        return make_shared<LayoutComponent>(
            LayoutParams{
                    paddingLeft,
                    paddingTop,
                    paddingRight,
                    paddingBottom,
                    verticalLayoutType,
                    horizontalLayoutType,
                    originVerticalLayoutType,
                    originHorizontalLayoutType,
                    referenceViewBounds
            },
            m_unitsConverter
            );
    }
    else if (type == "GestureConsumer") {
        auto priorityJson = componentJson["priority"];
        if (!priorityJson.is_number()) {
            throw domain_error("Can't find priority for gesture consumer");
        }
        auto gestureConsumer = make_shared<GestureConsumerComponent>(
            m_gesturesDispatcher,
            priorityJson.get<int>()
            );
        m_gesturesDispatcher->addGestureConsumer(gestureConsumer);
        return gestureConsumer;
    }
    else if (type == "ClickDetector") {
        return make_shared<ClickDetectorComponent>();
    }
    else if (type == "TextButton") {
        auto textColor = parseColor4f(componentJson["textColor"]);
        auto pressedTextColor = parseColor4f(componentJson["pressedTextColor"]);
        return make_shared<TextButtonComponent>(textColor, pressedTextColor);
    }
    else if (type == "ImageButton") {
        auto materialNameJson = componentJson["materialName"];
        if (!materialNameJson.is_string()) {
            throw domain_error("Malformed material name for image button");
        }
        auto pressedMaterialNameJson = componentJson["pressedMaterialName"];
        if (!pressedMaterialNameJson.is_string()) {
            throw domain_error("Malformed pressed material name for image button");
        }
        return make_shared<ImageButtonComponent>(
            materialsMap.at(materialNameJson.get<string>()),
            materialsMap.at(pressedMaterialNameJson.get<string>())
            );
    }
    else if (type == "Transform2D") {
        ComplexValue positionX;
        ComplexValue positionY;
        ComplexValue scaleX;
        ComplexValue scaleY;

        auto positionJson = componentJson["position"];
        if (positionJson.is_array() && positionJson.size() == 2) {
            positionX = parseComplexValue(positionJson[0], DimensionType::WIDTH);
            positionY = parseComplexValue(positionJson[1], DimensionType::HEIGHT);
        }
        else {
            throw domain_error("Malformed Transform2D position");
        }

        auto scaleJson = componentJson["scale"];
        if (scaleJson.is_array() && scaleJson.size() == 2) {
            scaleX = parseComplexValue(scaleJson[0], DimensionType::WIDTH);
            scaleY = parseComplexValue(scaleJson[1], DimensionType::HEIGHT);
        }
        else {
            throw domain_error("Malformed Transform2D scale");
        }

        return make_shared<Transformation2DComponent>(
            m_unitsConverter,
            m_displayInfo,
            positionX,
            positionY,
            scaleX,
            scaleY
            );
    }*/
    else if (type == "PerspectiveCamera") {
        auto camera = m_serviceLocator->cameraComponentsManager()->createPerspectiveCamera(
            m_serviceLocator,
            parseColor4f(componentJson["clearColor"]),
            parseLayerNames(componentJson["layerNames"]),
            parseFloatNumber(componentJson["fov"]),
            parseFloatNumber(componentJson["zNear"]),
            parseFloatNumber(componentJson["zFar"]),
            componentJson["order"].get<int>()
        );
        camera->setShouldClearColor(
            componentJson.contains("shouldClearColor") &&
            componentJson["shouldClearColor"].is_boolean() &&
            componentJson["shouldClearColor"].get<bool>()
        );
        camera->setShouldClearDepth(
            componentJson.contains("shouldClearDepth") &&
            componentJson["shouldClearDepth"].is_boolean() &&
            componentJson["shouldClearDepth"].get<bool>()
        );
        return camera;
    } else if (type == "DirectionalLight") {
        return m_serviceLocator->lightComponentsManager()->createDirectionalLight(
            parseColor3f(componentJson["color"]),
            Constants::DEFAULT_FORWARD_DIRECTION,
            parseLayerNames(componentJson["layerNames"])
        );
    }
    /*else if (type == "ScrollDetector") {
        return make_shared<ScrollDetectorComponent>();
    }*/
    else if (type == "SphereRigidBody") {
        auto mass = componentJson.contains("mass") ? parseFloatNumber(componentJson["mass"]) : optional<float>();
        auto radius = parseFloatNumber(componentJson["radius"]);
        auto sphereRigidBodyComponent = make_shared<SphereRigidBodyComponent>(
            m_serviceLocator->physicsEngine(),
            mass,
            radius
        );
        return sphereRigidBodyComponent;
    } else if (type == "CylinderRigidBody") {
        auto mass = componentJson.contains("mass") ? parseFloatNumber(componentJson["mass"]) : optional<float>();
        auto radius = parseFloatNumber(componentJson["radius"]);
        auto length = parseFloatNumber(componentJson["length"]);
        auto cylinderRigidBodyComponent = make_shared<CylinderRigidBodyComponent>(
            m_serviceLocator->physicsEngine(),
            mass,
            radius,
            length
        );
        return cylinderRigidBodyComponent;
    } else if (type == "TriMeshRigidBody") {
        auto mass = componentJson.contains("mass") ? parseFloatNumber(componentJson["mass"]) : optional<float>();
        auto meshName = componentJson["meshName"].get<string>();

        auto transform = static_pointer_cast<TransformationComponent>(
            gameObject->findComponent(TransformationComponent::TYPE_NAME)
        );

        auto meshPosition = parseColor3f(componentJson["meshPosition"]);
        auto meshRotationAngles = parseColor3f(componentJson["meshRotation"]);
        auto meshScale = parseColor3f(componentJson["meshScale"]);

        auto rigidBodyName = gameObject->name();

        auto rotationMatrix = glm::identity<glm::mat4>();
        rotationMatrix = glm::rotate(rotationMatrix, glm::radians(meshRotationAngles.z), glm::vec3(0, 0, 1));
        rotationMatrix = glm::rotate(rotationMatrix, glm::radians(meshRotationAngles.x), glm::vec3(1, 0, 0));
        rotationMatrix = glm::rotate(rotationMatrix, glm::radians(meshRotationAngles.y), glm::vec3(0, 1, 0));
        auto meshRotation = glm::quat_cast(rotationMatrix);

        auto mesh = createTransformedMesh(meshStorage.getMesh(meshName), meshPosition, meshRotation, meshScale);

        auto triMeshRigidBody = make_shared<TriMeshRigidBodyComponent>(m_serviceLocator->physicsEngine(), mass, mesh);

        return triMeshRigidBody;
    }/*
    else if (type == "CollisionsInfoContainer") {
        return make_shared<CollisionsInfoComponent>();
    }
    else if (type == "CharacterCapsuleRigidBody") {
        auto mass = componentJson.contains("mass") ? parseFloatNumber(componentJson["mass"]) : optional<float>();
        auto radius = parseFloatNumber(componentJson["radius"]);
        auto height = parseFloatNumber(componentJson["height"]);
        auto transform = static_pointer_cast<TransformationComponent>(
            gameObject->findComponent(TransformationComponent::TYPE_NAME)
            );
        auto rigidBodyName = gameObject->name();
        g_physicsEngine->createCharacterCapsuleRigidBody(
            gameObject,
            rigidBodyName,
            mass,
            radius,
            height,
            transform->position(),
            0,
            0,
            0
        );
        return make_shared<RigidBodyComponent>(
            rigidBodyName,
            g_physicsEngine
            );
    }
    else if (type == "SkeletalAnimation") {
        const string& name = componentJson["animationName"].get<string>();
        auto optionalSkeletalAnimation = m_skeletalAnimationStorage.findAnimation(name);
        if (!optionalSkeletalAnimation) {
            stringstream ss;
            ss << "Skeletal animation " << name << " not found";
            throw domain_error(ss.str());
        }
        return make_shared<SkeletalAnimationComponent>(
            optionalSkeletalAnimation.value(),
            name
            );
    }
    else if (type == "SkeletalAnimationPlayer") {
        return make_shared<SkeletalAnimationPlayerComponent>(m_time);
    }
    else if (type == "SoundListener") {
        return make_shared<SoundListenerComponent>(m_soundScene);
    }
    else if (type == "SoundPlayer") {
        return make_shared<SoundPlayerComponent>(
            m_soundScene,
            componentJson["soundName"].get<string>(),
            componentJson["gain"].get<float>(),
            componentJson["pitch"].get<float>(),
            componentJson["rolloffFactor"].get<float>(),
            componentJson["referenceDistance"].get<float>(),
            componentJson["maxDistance"].get<float>(),
            componentJson["isLooped"].get<bool>(),
            componentJson.contains("isRelativeToListener") && componentJson["isRelativeToListener"].get<bool>()
            );
    }*/
    else {
        stringstream ss;
        ss << "Unknown component type " << type;
        throw domain_error(ss.str());
    }
}
