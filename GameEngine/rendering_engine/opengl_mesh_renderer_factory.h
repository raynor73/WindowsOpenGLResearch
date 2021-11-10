#pragma once

#include <memory>
#include <rendering_engine/opengl_error_detector.h>
#include <rendering_engine/opengl_geometry_buffers_storage.h>
#include <rendering_engine/opengl_textures_repository.h>
#include <game_engine/mesh_renderer_factory.h>
#include <game_engine/without_generated_methods.h>
#include <unordered_map>
#include <rendering_engine/opengl_mesh_renderer_component.h>

namespace GameEngine
{
class OpenGLMeshRendererFactory : public MeshRendererFactory, public WithoutGeneratedMethods
{
    std::shared_ptr<OpenGLErrorDetector> m_openGLErrorDetector;
    std::shared_ptr<OpenGLGeometryBuffersStorage> m_geometryBuffersStorage;
    OpenGLTexturesRepository* m_texturesRepository;

    std::unordered_multimap<std::string, std::shared_ptr<OpenGLMeshRendererComponent>> m_layerNameToMeshRenderersMap;
    std::unordered_multimap<std::shared_ptr<OpenGLMeshRendererComponent>, std::string> m_meshRendererToLayerNamesMap;
    //std::unordered_multimap<std::string, std::shared_ptr<OpenGLMeshRendererComponent>> layerNameToTranslucentMeshRenderersMap;

public:
    OpenGLMeshRendererFactory(
        std::shared_ptr<OpenGLGeometryBuffersStorage> geometryBuffersStorage,
        OpenGLTexturesRepository* texturesRepository,
        std::shared_ptr<OpenGLErrorDetector> openGlErrorDetector
    ) : m_geometryBuffersStorage(geometryBuffersStorage),
        m_texturesRepository(texturesRepository),
        m_openGLErrorDetector(openGlErrorDetector)
    {}

    const std::unordered_multimap<std::string, std::shared_ptr<OpenGLMeshRendererComponent>>& layerNameToMeshRenderersMap() { return m_layerNameToMeshRenderersMap; };

    virtual std::shared_ptr<GameObjectComponent> createMeshRenderer(std::vector<std::string> layerNames) override;
    virtual void releaseMeshRenderer(std::shared_ptr<GameObjectComponent> meshRenderer) override;

    virtual void reset() override;
};
}
