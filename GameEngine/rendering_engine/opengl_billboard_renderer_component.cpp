#include "opengl_billboard_renderer_component.h"

using namespace GameEngine;
using namespace std;

shared_ptr<GameObjectComponent> OpenGLBillboardRendererComponent::clone()
{
    /*auto clone = new OpenGLMeshRendererComponent(
        m_layerNames,
        m_geometryBuffersStorage,
        m_texturesRepository,
        m_openGLErrorDetector
    );
    clone->setEnabled(m_isEnabled);*/
    return shared_ptr<OpenGLBillboardRendererComponent>();
}
