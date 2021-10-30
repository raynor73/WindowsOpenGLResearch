#pragma once

#include <vector>
#include <string>
#include <memory>
#include <glm/vec3.hpp>
#include <rendering_engine/opengl_error_detector.h>
#include <rendering_engine/opengl_geometry_buffers_storage.h>
#include <rendering_engine/opengl_textures_repository.h>
#include <game_engine/game_object_component.h>
#include <game_engine/mesh.h>
#include <rendering_engine/opengl_shader_program_container.h>

namespace GameEngine
{
class OpenGLMeshRendererFactory;

class OpenGLMeshRendererComponent : public GameObjectComponent
{
    std::vector<std::string> m_layerNames;
    std::shared_ptr<OpenGLErrorDetector> m_openGLErrorDetector;
    std::shared_ptr<OpenGLGeometryBuffersStorage> m_geometryBuffersStorage;
    OpenGLTexturesRepository* m_texturesRepository;

    glm::vec3 m_topPoint = glm::vec3(0);
    glm::vec3 m_bottomPoint = glm::vec3(0);
    bool m_isTopAndBottomPointsFound = false;

    friend class OpenGLMeshRendererFactory;

public:
    static const std::string TYPE_NAME;

    const std::vector<std::string>& layerNames() const { return m_layerNames; }
    //void setLayerNames(std::vector<std::string> layerNames) { m_layerNames = std::move(layerNames); }

    void render(
        const OpenGLShaderProgramContainer& shaderProgramContainer,
        const glm::mat4x4& modelMatrix,
        const glm::mat4x4& viewMatrix,
        const glm::mat4x4& projectionMatrix
    );

    virtual void onAttachedToGameObject() override;

    virtual const std::string& typeName() const override { return TYPE_NAME; }

    virtual std::shared_ptr<GameObjectComponent> clone() override;

private:
    OpenGLMeshRendererComponent(
        std::vector<std::string> layerNames,
        std::shared_ptr<OpenGLGeometryBuffersStorage> geometryBuffersStorage,
        OpenGLTexturesRepository* texturesRepository,
        std::shared_ptr<OpenGLErrorDetector> openGLErrorDetector
    ) : m_layerNames(std::move(layerNames)),
        m_geometryBuffersStorage(std::move(geometryBuffersStorage)),
        m_texturesRepository(std::move(texturesRepository)),
        m_openGLErrorDetector(std::move(openGLErrorDetector))
    {}

    void findTopAndBottomPoints(const Mesh& mesh);
    void putMeshInGeometryBuffersIfNecessary(const std::string& name, const Mesh& mesh);
};
}
