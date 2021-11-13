#pragma once

#include <vector>
#include <string>
#include <memory>
#include <glm/vec3.hpp>
#include <rendering_engine/opengl_error_detector.h>
#include <rendering_engine/opengl_textures_repository.h>
#include <game_engine/game_object_component.h>
#include <glm/glm.hpp>
#include <rendering_engine/opengl_shader_program_container.h>

namespace GameEngine
{
class OpenGLBillboardRendererFactory;

class OpenGLBillboardRendererComponent : public GameObjectComponent
{
    std::vector<std::string> m_layerNames;
    std::shared_ptr<OpenGLErrorDetector> m_openGLErrorDetector;
    // The cause of usage of the Textures Repository by pointer is becuase originally it is provided by
    // Service Locatro which provides services as pointers. Probably it is good idea to refactor Service Locator to
    // return Shared Pointers instead of raw pointers.
    OpenGLTexturesRepository* m_texturesRepository;

    friend class OpenGLBillboardRendererFactory;

public:
    static const std::string TYPE_NAME;

    const std::vector<std::string>& layerNames() const { return m_layerNames; }

    void render(
        const OpenGLShaderProgramContainer& shaderProgramContainer,
        const glm::mat4x4& modelMatrix,
        const glm::mat4x4& viewMatrix,
        const glm::mat4x4& projectionMatrix
    );

    virtual const std::string& typeName() const override { return TYPE_NAME; }

    virtual std::shared_ptr<GameObjectComponent> clone() override;

private:
    OpenGLBillboardRendererComponent(
        std::vector<std::string> layerNames,
        OpenGLTexturesRepository* texturesRepository,
        std::shared_ptr<OpenGLErrorDetector> openGLErrorDetector
    ) : m_layerNames(std::move(layerNames)),
        m_texturesRepository(std::move(texturesRepository)),
        m_openGLErrorDetector(std::move(openGLErrorDetector))
    {}
};
}
