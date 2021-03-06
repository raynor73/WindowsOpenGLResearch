#include "opengl_rendering_engine.h"
#include <game_engine/logger.h>
#include <game_engine/constants.h>
#include <game_engine/ortho_camera_component.h>
#include <game_engine/perspective_camera_component.h>
#include <game_engine/utils.h>
#include <game_engine/material_component.h>
#include <game_engine/mesh_component.h>
#include <algorithm>
#include <game_engine/transformation_component.h>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <game_engine/directional_light_component.h>

using namespace GameEngine;
using namespace std;

OpenGLRenderingEngine::OpenGLRenderingEngine(
    shared_ptr<OpenGLErrorDetector> openGLErrorDetector,
    UnitsConverter* unitsConverter,
    shared_ptr<OpenGLShadersRepository> shadersRepository,
    shared_ptr<OpenGLShaderSourcePreprocessor> shaderSourcePreprocessor,
    shared_ptr<OpenGLGeometryBuffersStorage> geometryBuffersStorage,
    OpenGLTexturesRepository* texturesRepository,
    CameraComponentsManager* cameraComponentsManager,
    OpenGLMeshRendererFactory* meshRendererFactory,
    LightComponentsManager* lightComponentsManager
) : m_openGLErrorDetector(openGLErrorDetector),
    m_unitsConverter(unitsConverter),
    m_shadersRepository(shadersRepository),
    m_geometryBuffersStorage(geometryBuffersStorage),
    m_texturesRepository(texturesRepository),
    m_cameraComponentsManager(cameraComponentsManager),
    m_meshRendererFactory(meshRendererFactory),
    m_lightComponentsManager(lightComponentsManager),
    m_isErrorLogged(false)
{
    /*auto unlitVertexShaderSource = shaderSourcePreprocessor->loadShaderSource(
        "shaders/unlit/unlitVertexShader.glsl"
    );
    auto unlitFragmentShaderSource = shaderSourcePreprocessor->loadShaderSource(
        "shaders/unlit/unlitFragmentShader.glsl"
    );
    shadersRepository->createVertexShader("unlit", unlitVertexShaderSource);
    shadersRepository->createFragmentShader("unlit", unlitFragmentShaderSource);
    shadersRepository->createShaderProgram("unlit", "unlit", "unlit");

    auto textVertexShaderSource = shaderSourcePreprocessor->loadShaderSource(
        "shaders/text/textVertexShader.glsl"
    );
    auto textFragmentShaderSource = shaderSourcePreprocessor->loadShaderSource(
        "shaders/text/textFragmentShader.glsl"
    );
    shadersRepository->createVertexShader("text", textVertexShaderSource);
    shadersRepository->createFragmentShader("text", textFragmentShaderSource);
    shadersRepository->createShaderProgram("text", "text", "text");*/

    auto ambientVertexShaderSource = shaderSourcePreprocessor->loadShaderSource(
        "shaders/light/ambient_vertex_shader.glsl"
    );
    auto ambientFragmentShaderSource = shaderSourcePreprocessor->loadShaderSource(
        "shaders/light/ambient_fragment_shader.glsl"
    );
    shadersRepository->createVertexShader("ambient", ambientVertexShaderSource);
    shadersRepository->createFragmentShader("ambient", ambientFragmentShaderSource);
    shadersRepository->createShaderProgram("ambient", "ambient", "ambient");

    auto directionalLightVertexShaderSource = shaderSourcePreprocessor->loadShaderSource(
        "shaders/light/directional_light_vertex_shader.glsl"
    );
    auto directionalLightFragmentShaderSource = shaderSourcePreprocessor->loadShaderSource(
        "shaders/light/directional_light_fragment_shader.glsl"
    );
    shadersRepository->createVertexShader("directionalLight", directionalLightVertexShaderSource);
    shadersRepository->createFragmentShader("directionalLight", directionalLightFragmentShaderSource);
    shadersRepository->createShaderProgram("directionalLight", "directionalLight", "directionalLight");

    setupOpenGL();
}

OpenGLRenderingEngine::~OpenGLRenderingEngine()
{
    reset();
    m_shadersRepository->removeAllShadersAndPrograms();
}

void OpenGLRenderingEngine::render(Scene& scene) {
    if (m_openGLErrorDetector->isOpenGLErrorDetected()) {
        if (!m_isErrorLogged) {
            m_isErrorLogged = true;
            L::e(
                Constants::LOG_TAG,
                "Rendering Engine will not render anymore as OpenGL error have been detected"
            );
        }

        return;
    }

    //traverseSceneHierarchy(*scene.rootGameObject(), [&](GameObject& gameObject) {
        // TODO This is very very bad to update physics related components in Rendering Engine. This added here to not to traverse whole hierarchy multiple times. But this should be moved out from here ASAP.
        /*if (auto collisionsInfo = gameObject.findComponent<CollisionsInfoComponent>(); collisionsInfo != nullptr) {
            collisionsInfo->collisions.clear();
        }*/

        /*if (
            auto textRenderer = gameObject.findComponent<OpenGLFreeTypeTextRendererComponent>();
            textRenderer != nullptr && textRenderer->isEnabled()
            ) {
            for (auto& layerName : textRenderer->layerNames()) {
                layerNameToTextRenderersMap.insert({ layerName, textRenderer });
            }
        }*/
    //});

    for (auto& camera : m_cameraComponentsManager->cameras()) {
        if (!camera->isEnabled()) {
            continue;
        }

        GLbitfield clearMask = 0;
        if (camera->shouldClearColor()) {
            clearMask |= GL_COLOR_BUFFER_BIT;
        }
        if (camera->shouldClearDepth()) {
            clearMask |= GL_DEPTH_BUFFER_BIT;
        }

        int viewportX = int(m_unitsConverter->complexValueToPixels(camera->viewportX()));
        int viewportY = int(m_unitsConverter->complexValueToPixels(camera->viewportY()));
        int viewportWidth = int(m_unitsConverter->complexValueToPixels(camera->viewportWidth()));
        int viewportHeight = int(m_unitsConverter->complexValueToPixels(camera->viewportHeight()));
        Viewport viewport{ viewportX, viewportY, viewportWidth, viewportHeight };
        Scissor scissor{ viewportX, viewportY, viewportWidth, viewportHeight };
        pushOpenGLState({
                viewport,
                scissor,
                true,
                {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA},
                true,
                GL_LESS
            });

        auto clearColor = camera->clearColor();
        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(clearMask);

        for (auto& layerName : camera->layerNames()) {
            auto layerRenderersRange = m_meshRendererFactory->layerNameToMeshRenderersMap().equal_range(layerName);
            for (auto it = layerRenderersRange.first; it != layerRenderersRange.second; it++) {
                if (!it->second->isEnabled()) {
                    continue;
                }

                renderMeshWithAllRequiredShaders(
                    camera,
                    viewport,
                    scissor,
                    it->second,
                    m_lightComponentsManager->layerNameToAmbientLightMap(),
                    m_lightComponentsManager->layerNameToDirectionalLightsMap(),
                    layerName
                );
            }

            /*auto layerTranslucentRenderersRange = layerNameToTranslucentMeshRenderersMap.equal_range(layerName);
            for (auto it = layerTranslucentRenderersRange.first; it != layerTranslucentRenderersRange.second; it++) {
                renderMeshWithAllRequiredShaders(
                    camera,
                    viewport,
                    scissor,
                    it->second,
                    layerNameToAmbientLightMap,
                    layerNameToDirectionalLightsMap,
                    layerName
                );
            }*/

            /*auto layerTextRenderersRange = layerNameToTextRenderersMap.equal_range(layerName);
            for (auto it = layerTextRenderersRange.first; it != layerTextRenderersRange.second; it++) {
                renderText(camera, it->second);
            }*/
        }

        popOpenGLState();
    }
}

void OpenGLRenderingEngine::reset()
{
    m_geometryBuffersStorage->removeAllBuffers();
    m_texturesRepository->removeAllTextures();
    setupOpenGL();
}

void OpenGLRenderingEngine::renderMeshWithAllRequiredShaders(
    const shared_ptr<CameraComponent>& camera,
    const Viewport& viewport,
    const Scissor& scissor,
    const shared_ptr<OpenGLMeshRendererComponent>& meshRenderer,
    const unordered_map<string, shared_ptr<AmbientLightComponent>>& layerNameToAmbientLightMap,
    const unordered_multimap<string, shared_ptr<DirectionalLightComponent>>& layerNameToDirectionalLightsMap,
    const string& layerName
) {
    auto material = meshRenderer->gameObject().lock()->findComponent<MaterialComponent>()->material();
    if (material.isUnlit) {
        auto shaderProgramContainer = m_shadersRepository->getShaderProgramContainer("unlit");
        glUseProgram(shaderProgramContainer.shaderProgram());
        renderMesh(camera, meshRenderer, shaderProgramContainer);
    } else {
        const auto& ambientLight = layerNameToAmbientLightMap.at(layerName);
        Utils::throwErrorIfNull(ambientLight, "No ambient light found");
        auto shaderProgramContainer = m_shadersRepository->getShaderProgramContainer("ambient");
        glUseProgram(shaderProgramContainer.shaderProgram());
        if (auto ambientColorUniform = shaderProgramContainer.ambientColorUniform(); ambientColorUniform >= 0) {
            auto ambientColor = ambientLight->color();
            glUniform3f(
                ambientColorUniform,
                ambientColor.r,
                ambientColor.g,
                ambientColor.b
            );
        }
        renderMesh(camera, meshRenderer, shaderProgramContainer);

        pushOpenGLState({
                viewport,
                scissor,
                true,
                {GL_ONE, GL_ONE},
                false,
                GL_EQUAL
            });

        shaderProgramContainer = m_shadersRepository->getShaderProgramContainer("directionalLight");
        glUseProgram(shaderProgramContainer.shaderProgram());

        auto layerDirectionalLightsRange = layerNameToDirectionalLightsMap.equal_range(layerName);
        for (auto it = layerDirectionalLightsRange.first; it != layerDirectionalLightsRange.second; it++) {
            if (!it->second->isEnabled()) {
                continue;
            }

            if (auto colorUniform = shaderProgramContainer.directionalLightColorUniform(); colorUniform >= 0) {
                auto color = it->second->color();
                glUniform3f(
                    colorUniform,
                    color.r,
                    color.g,
                    color.b
                );
            }
            if (auto directionUniform = shaderProgramContainer.directionalLightDirectionUniform(); directionUniform >= 0) {
                auto transform = it->second->gameObject().lock()->findComponent<TransformationComponent>();
                Utils::throwErrorIfNull(transform, "Directional light have no transform while rendering");
                auto direction = transform->rotation() * it->second->direction();
                glUniform3f(
                    directionUniform,
                    direction.x,
                    direction.y,
                    direction.z
                );
            }
            renderMesh(camera, meshRenderer, shaderProgramContainer);
        }

        popOpenGLState();
    }
}

void OpenGLRenderingEngine::renderMesh(
    const shared_ptr<CameraComponent>& camera,
    const shared_ptr<OpenGLMeshRendererComponent>& meshRenderer,
    const OpenGLShaderProgramContainer& shaderProgramContainer
) {
    auto gameObject = meshRenderer->gameObject().lock();
    if (gameObject == nullptr) {
        throw domain_error("Mesh renderer is not attached to any game object");
    }

    auto transform = static_pointer_cast<TransformationComponent>(
        gameObject->findComponent(TransformationComponent::TYPE_NAME)
    );
    if (transform == nullptr) {
        stringstream ss;
        ss << "Being rendered game object " << gameObject->name() << " has no transform component";
        throw domain_error(ss.str());
    }

    auto modelMatrix = glm::translate(glm::identity<glm::mat4>(), transform->position());
    modelMatrix *= glm::toMat4(transform->rotation());
    modelMatrix = glm::scale(modelMatrix, transform->scale());

    meshRenderer->render(
        shaderProgramContainer,
        modelMatrix,
        camera->calculateViewMatrix(),
        camera->calculateProjectionMatrix()
    );
}

/*void OpenGLRenderingEngine::renderText(
    const shared_ptr<CameraComponent>& camera,
    const shared_ptr<OpenGLFreeTypeTextRendererComponent>& textRenderer
) {
    auto gameObject = textRenderer->gameObject();
    if (gameObject == nullptr) {
        throw domain_error("Text renderer is not attached to any game object");
    }

    auto transform = static_pointer_cast<TransformationComponent>(
        gameObject->findComponent(TransformationComponent::TYPE_NAME)
        );
    if (transform == nullptr) {
        stringstream ss;
        ss << "Text's game object " << gameObject->name() << " has no transform component";
        throw domain_error(ss.str());
    }

    auto shaderProgramContainer = m_shadersRepository->getShaderProgramContainer("text");
    glUseProgram(shaderProgramContainer.shaderProgram());

    auto modelMatrix = glm::translate(glm::identity<glm::mat4>(), transform->position());
    modelMatrix *= glm::toMat4(transform->rotation());
    modelMatrix = glm::scale(modelMatrix, transform->scale());

    textRenderer->render(
        shaderProgramContainer,
        modelMatrix,
        camera->calculateViewMatrix(),
        camera->calculateProjectionMatrix()
    );
}*/

void OpenGLRenderingEngine::pushOpenGLState(const OpenGLState& state) {
    applyOpenGLState(state);
    m_openGLStateStack.push(state);
}

void OpenGLRenderingEngine::popOpenGLState() {
    m_openGLStateStack.pop();
    if (!m_openGLStateStack.empty()) {
        applyOpenGLState(m_openGLStateStack.top());
    }
}

void OpenGLRenderingEngine::applyOpenGLState(const OpenGLState& state) {
    glViewport(state.viewport.x, state.viewport.y, state.viewport.width, state.viewport.height);
    glScissor(state.scissor.x, state.scissor.y, state.scissor.width, state.scissor.height);
    if (state.blend) {
        glEnable(GL_BLEND);
    }
    else {
        glDisable(GL_BLEND);
    }
    glBlendFunc(state.blendFunction.sFactor, state.blendFunction.dFactor);
    glDepthMask(state.depthMask);
    glDepthFunc(state.depthFunction);

    m_openGLErrorDetector->checkOpenGLErrors("OpenGLRenderingEngine::applyOpenGLState");
}

void OpenGLRenderingEngine::setupOpenGL() {
    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CCW);
    glEnable(GL_SCISSOR_TEST);
    glClearDepthf(1);
}
