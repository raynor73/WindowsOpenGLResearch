#pragma once

#include <vector>
#include <string>
#include <memory>
#include <glm/vec3.hpp>
#include <rendering_engine/opengl_error_detector.h>
#include <rendering_engine/opengl_geometry_buffers_storage.h>

namespace GameEngine
{
namespace RenderingEngine
{
class OpenGlMeshRendererComponent
{
    std::vector<std::string> m_layerNames;
    std::shared_ptr<OpenGLErrorDetector> m_openGlErrorDetector;
    std::shared_ptr<OpenGLGeometryBuffersStorage> m_geometryBuffersStorage;
    std::shared_ptr<OpenGLTexturesRepository> m_texturesRepository;

    glm::vec3 m_topPoint;
    glm::vec3 m_bottomPoint;
    bool m_isTopAndBottomPointsFound = false;
};
}
}
