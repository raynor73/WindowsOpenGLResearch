#include "opengl_mesh_renderer_factory.h"
#include <rendering_engine/opengl_mesh_renderer_component.h>

using namespace GameEngine;
using namespace GameEngine::RenderingEngine;
using namespace std;

shared_ptr<GameObjectComponent> OpenGLMeshRendererFactory::createMeshRenderer(vector<string> layerNames)
{
    return std::make_shared<OpenGLMeshRendererComponent>(
        layerNames,
        m_geometryBuffersStorage,
        m_texturesRepository,
        m_openGLErrorDetector
    );
}
