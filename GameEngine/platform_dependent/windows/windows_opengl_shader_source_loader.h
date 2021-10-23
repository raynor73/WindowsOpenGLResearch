#pragma once

#include <rendering_engine/opengl_shader_source_loader.h>
#include <game_engine/without_generated_methods.h>
#include <memory>

namespace GameEngine
{
class ServiceLocator;

namespace RenderingEngine
{
class WindowsOpenGLShaderSourceLoader : public OpenGLShaderSourceLoader, public WithoutGeneratedMethods
{
    std::shared_ptr<ServiceLocator> m_serviceLocator;
public:
    explicit WindowsOpenGLShaderSourceLoader(std::shared_ptr<ServiceLocator> serviceLocator) : m_serviceLocator(serviceLocator) {}

    virtual std::string loadShaderSource(const std::string& path) override;
};
}
}
