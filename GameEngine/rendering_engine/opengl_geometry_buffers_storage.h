#pragma once

#include <memory>
#include <unordered_map>
#include <string>
#include <GL/glew.h>
#include <rendering_engine/opengl_error_detector.h>
#include <rendering_engine/ibo_info.h>
#include <game_engine/without_generated_methods.h>
#include <optional>

namespace GameEngine
{
class OpenGLGeometryBuffersStorage : public WithoutGeneratedMethods
{
    std::shared_ptr<OpenGLErrorDetector> m_openGLErrorDetector;

    std::unordered_map<std::string, GLuint> m_vbos;
    std::unordered_map<std::string, IboInfo> m_ibos;

public:
    OpenGLGeometryBuffersStorage(std::shared_ptr<OpenGLErrorDetector> openGLErrorDetector) :
        m_openGLErrorDetector(std::move(openGLErrorDetector)) {}

    GLuint createStaticVertexBuffer(const std::string& name, const std::vector<float>& vertexData);
    IboInfo createStaticIndexBuffer(const std::string& name, const std::vector<uint16_t>& indices);

    std::optional<GLuint> findVbo(const std::string& name);
    std::optional<IboInfo> findIbo(const std::string& name);

    void removeStaticVertexBuffer(const std::string& name);
    void removeStaticIndexBuffer(const std::string& name);

    std::vector<std::string> retrieveStoredVboNames() const;
    std::vector<std::string> retrieveStoredIboNames() const;

    void removeAllBuffers();
};
}
