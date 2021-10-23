#pragma once

#include <string>

namespace GameEngine
{
namespace RenderingEngine
{
class OpenGLShaderSourceLoader
{
public:
    virtual ~OpenGLShaderSourceLoader() = default;

    virtual std::string loadShaderSource(const std::string& path) = 0;
};
}
}
