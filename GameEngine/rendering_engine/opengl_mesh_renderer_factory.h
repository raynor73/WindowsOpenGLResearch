#pragma once

#include <memory>
#include <rendering_engine/opengl_error_detector.h>
#include <rendering_engine/opengl_geometry_buffers_storage.h>
#include <rendering_engine/opengl_textures_repository.h>
#include <game_engine/mesh_renderer_factory.h>
#include <game_engine/without_generated_methods.h>

namespace GameEngine
{
namespace RenderingEngine
{
class OpenGLMeshRendererFactory : public MeshRendererFactory, public WithoutGeneratedMethods
{
    std::shared_ptr<OpenGLErrorDetector> m_openGLErrorDetector;
    std::shared_ptr<OpenGLGeometryBuffersStorage> m_geometryBuffersStorage;
    OpenGLTexturesRepository* m_texturesRepository;

public:
    OpenGLMeshRendererFactory(
        std::shared_ptr<OpenGLGeometryBuffersStorage> geometryBuffersStorage,
        OpenGLTexturesRepository* texturesRepository,
        std::shared_ptr<OpenGLErrorDetector> openGlErrorDetector
    ) : m_geometryBuffersStorage(geometryBuffersStorage),
        m_texturesRepository(texturesRepository),
        m_openGLErrorDetector(openGlErrorDetector)
    {}

    virtual std::shared_ptr<GameObjectComponent> createMeshRenderer(std::vector<std::string> layerNames) override;
};
}
}
