#pragma once

#include <memory>
#include <string>
#include <rendering_engine/opengl_shader_source_loader.h>
#include <game_engine/without_generated_methods.h>

namespace GameEngine
{
namespace RenderingEngine 
{
class OpenGLShaderSourcePreprocessor : public WithoutGeneratedMethods
{
    std::shared_ptr<OpenGLShaderSourceLoader> m_shaderSourceLoader;

public:
    OpenGLShaderSourcePreprocessor(std::shared_ptr<OpenGLShaderSourceLoader> shaderSourceLoader) :
        m_shaderSourceLoader(shaderSourceLoader) {}

    std::string loadShaderSource(const std::string& path);

private:
    static const std::string INCLUDE_TOKEN_PREFIX;
    static const std::string INCLUDE_TOKEN_POSTFIX;
};
}
}
