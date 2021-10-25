#pragma once

#include <game_engine/scene_manager.h>
#include <game_engine/time_manager.h>
#include <game_engine/rendering_window_info_provider.h>
#include <game_engine/read_only_fs_abstraction.h>
#include <game_engine/fs_abstraction.h>
#include <game_engine/mesh_loader.h>
#include <game_engine/mesh_renderer_factory.h>
#include <game_engine/units_converter.h>
#include <game_engine/time_provider.h>
#include <game_engine/scene_hierarchy_loader.h>
#include <game_engine/bitmap_loader.h>
#include <game_engine/textures_repository.h>
#include <game_engine/input/keyboard_input.h>
#include <game_engine/app.h>
#include <game_engine/input/mouse_input.h>
#include <memory>
#include <game_engine/physics_engine.h>
#include <game_engine/rendering_engine.h>

namespace GameEngine
{
class ServiceLocator
{
public:
    void provide(std::shared_ptr<SceneManager> sceneManager);
    void provide(std::shared_ptr<TimeProvider> timeProvider);
    void provide(std::shared_ptr<TimeManager> timeManager);
    void provide(std::shared_ptr<RenderingWindowInfoProvider> renderingWindowInfoProvider);
    void provide(std::shared_ptr<FsAbstraction> fsAbstraction);
    void provide(std::shared_ptr<ReadOnlyFsAbstraction> readOnlyFsAbstraction);
    void provide(std::shared_ptr<MeshLoader> meshLoader);
    void provide(std::shared_ptr<MeshRendererFactory> meshRendererFactory);
    void provide(std::shared_ptr<UnitsConverter> unitsConverter);
    void provide(std::shared_ptr<SceneHierarchyLoader> sceneHierarchyLoader);
    void provide(std::shared_ptr<BitmapLoader> bitmapLoader);
    void provide(std::shared_ptr<TexturesRepository> texturesRepository);
    void provide(std::shared_ptr<KeyboardInput> keyboardInput);
    void provide(std::shared_ptr<App> app);
    void provide(std::shared_ptr<MouseInput> mouseInput);
    void provide(std::shared_ptr<PhysicsEngine> physicsEngine);
    void provide(std::shared_ptr<RenderingEngine> renderingEngine);

    SceneManager* sceneManager();
    TimeProvider* timeProvider();
    TimeManager* timeManager();
    RenderingWindowInfoProvider* renderingWindowInfoProvider();
    FsAbstraction* fsAbstraction();
    ReadOnlyFsAbstraction* readOnlyFsAbstraction();
    MeshLoader* meshLoader();
    MeshRendererFactory* meshRendererFactory();
    UnitsConverter* unitsConverter();
    SceneHierarchyLoader* sceneHierarchyLoader();
    BitmapLoader* bitmapLoader();
    TexturesRepository* texturesRepository();
    KeyboardInput* keyboardInput();
    App* app();
    MouseInput* mouseInput();
    PhysicsEngine* physicsEngine();
    RenderingEngine* renderingEngine();

private:
    std::shared_ptr<SceneManager> m_sceneManager;
    std::shared_ptr<TimeProvider> m_timeProvider;
    std::shared_ptr<TimeManager> m_timeManager;
    std::shared_ptr<RenderingWindowInfoProvider> m_renderingWindowInfoProvider;
    std::shared_ptr<FsAbstraction> m_fsAbstraction;
    std::shared_ptr<ReadOnlyFsAbstraction> m_readOnlyFsAbstraction;
    std::shared_ptr<MeshLoader> m_meshLoader;
    std::shared_ptr<MeshRendererFactory> m_meshRendererFactory;
    std::shared_ptr<UnitsConverter> m_unitsConverter;
    std::shared_ptr<SceneHierarchyLoader> m_sceneHierarchyLoader;
    std::shared_ptr<BitmapLoader> m_bitmapLoader;
    std::shared_ptr<TexturesRepository> m_texturesRepository;
    std::shared_ptr<KeyboardInput> m_keyboardInput;
    std::shared_ptr<App> m_app;
    std::shared_ptr<MouseInput> m_mouseInput;
    std::shared_ptr<PhysicsEngine> m_physicsEngine;
    std::shared_ptr<RenderingEngine> m_renderingEngine;
};
}
