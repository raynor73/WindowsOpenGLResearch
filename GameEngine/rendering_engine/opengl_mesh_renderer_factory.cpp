#include "opengl_mesh_renderer_factory.h"
#include <rendering_engine/opengl_mesh_renderer_component.h>
#include <sstream>
#include <exception>

using namespace GameEngine;
using namespace std;

shared_ptr<GameObjectComponent> OpenGLMeshRendererFactory::createMeshRenderer(vector<string> layerNames)
{
    auto meshRenderer = new OpenGLMeshRendererComponent(
        layerNames,
        m_geometryBuffersStorage,
        m_texturesRepository,
        m_openGLErrorDetector
    );
    auto meshRendererSmartPtr = shared_ptr<OpenGLMeshRendererComponent>(meshRenderer);
    for (auto layerName : layerNames) {
        m_layerNameToMeshRenderersMap.insert({ layerName, meshRendererSmartPtr });
        m_meshRendererToLayerNamesMap.insert({ meshRendererSmartPtr, layerName });
    }
    return meshRendererSmartPtr;
}

void OpenGLMeshRendererFactory::releaseMeshRenderer(shared_ptr<GameObjectComponent> meshRenderer)
{
    if (meshRenderer->typeName() != OpenGLMeshRendererComponent::TYPE_NAME) {
        stringstream ss;
        ss << "Expected " << OpenGLMeshRendererComponent::TYPE_NAME << " but got " << meshRenderer->typeName();
        throw domain_error(ss.str());
    }

    auto castedMeshRenderer = static_pointer_cast<OpenGLMeshRendererComponent>(meshRenderer);
    auto layerNamesRange = m_meshRendererToLayerNamesMap.equal_range(castedMeshRenderer);

    for (auto layerNamesIterator = layerNamesRange.first; layerNamesIterator != layerNamesRange.second; ++layerNamesIterator) {
        auto meshRenderersRange = m_layerNameToMeshRenderersMap.equal_range(layerNamesIterator->second);
        for (auto meshRenderersIterator = meshRenderersRange.first; meshRenderersIterator != meshRenderersRange.second; ++meshRenderersIterator) {
            if (meshRenderersIterator->second == castedMeshRenderer) {
                m_layerNameToMeshRenderersMap.erase(meshRenderersIterator);
                break;
            }
        }
    }
    m_meshRendererToLayerNamesMap.erase(castedMeshRenderer);
}

void GameEngine::OpenGLMeshRendererFactory::reset()
{
    m_layerNameToMeshRenderersMap.clear();
    m_meshRendererToLayerNamesMap.clear();
}
