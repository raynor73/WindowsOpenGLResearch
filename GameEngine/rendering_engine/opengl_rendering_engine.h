#pragma once

#include <game_engine/without_generated_methods.h>
#include <memory>
#include <stack>
#include <rendering_engine/opengl_state.h>
#include <rendering_engine/opengl_error_detector.h>
#include <game_engine/units_converter.h>
#include <rendering_engine/opengl_shaders_repository.h>
#include <rendering_engine/opengl_shader_source_preprocessor.h>
#include <rendering_engine/opengl_geometry_buffers_storage.h>
#include <rendering_engine/opengl_textures_repository.h>
#include <game_engine/scene.h>
#include <functional>
#include <game_engine/game_object.h>
#include <game_engine/camera_component.h>
#include <rendering_engine/opengl_mesh_renderer_component.h>
#include <rendering_engine/opengl_shader_program_container.h>
#include <game_engine/ambient_light_component.h>
#include <game_engine/directional_light_component.h>
#include <game_engine/rendering_engine.h>
#include <game_engine/camera_components_manager.h>
#include <rendering_engine/opengl_mesh_renderer_factory.h>
#include <game_engine/light_components_manager.h>

namespace GameEngine
{
class OpenGLRenderingEngine : public RenderingEngine, public WithoutGeneratedMethods
{
    std::stack<OpenGLState> m_openGLStateStack;
    std::shared_ptr<OpenGLErrorDetector> m_openGLErrorDetector;
    UnitsConverter* m_unitsConverter;
    std::shared_ptr<OpenGLShadersRepository> m_shadersRepository;
    std::shared_ptr<OpenGLShaderSourcePreprocessor> m_shaderSourcePreprocessor;
    std::shared_ptr<OpenGLGeometryBuffersStorage> m_geometryBuffersStorage;
    OpenGLTexturesRepository* m_texturesRepository;
    CameraComponentsManager* m_cameraComponentsManager;
    OpenGLMeshRendererFactory* m_meshRendererFactory;
    LightComponentsManager* m_lightComponentsManager;

public:
    OpenGLRenderingEngine(
        std::shared_ptr<OpenGLErrorDetector> openGLErrorDetector,
        UnitsConverter* unitsConverter,
        std::shared_ptr<OpenGLShadersRepository> shadersRepository,
        std::shared_ptr<OpenGLShaderSourcePreprocessor> shaderSourcePreprocessor,
        std::shared_ptr<OpenGLGeometryBuffersStorage> geometryBuffersStorage,
        OpenGLTexturesRepository* texturesRepository,
        CameraComponentsManager* cameraComponentsManager,
        OpenGLMeshRendererFactory* OpenGLMeshRendererFactory,
        LightComponentsManager* lightComponentsManager
    );

    virtual ~OpenGLRenderingEngine() override;

    void render(Scene& scene);

    virtual void reset() override;

private:
    bool m_isErrorLogged;

    void renderMeshWithAllRequiredShaders(
        const std::shared_ptr<CameraComponent>& camera,
        const Viewport& viewport, const Scissor& scissor,
        const std::shared_ptr<OpenGLMeshRendererComponent>& meshRenderer,
        const std::unordered_map<std::string, std::shared_ptr<AmbientLightComponent>>& layerNameToAmbientLightMap,
        const std::unordered_multimap<std::string, std::shared_ptr<DirectionalLightComponent>>& layerNameToDirectionalLightsMap,
        const std::string& layerName
    );
    static void renderMesh(
        const std::shared_ptr<CameraComponent>& camera,
        const std::shared_ptr<OpenGLMeshRendererComponent>& meshRenderer,
        const OpenGLShaderProgramContainer& shaderProgramContainer
    );
    /*void renderText(
        const std::shared_ptr<CameraComponent>& camera,
        const std::shared_ptr<OpenGLFreeTypeTextRendererComponent>& textRenderer
    );*/

    void pushOpenGLState(const OpenGLState& state);
    void popOpenGLState();
    void applyOpenGLState(const OpenGLState& state);

    void setupOpenGL();
};
}
