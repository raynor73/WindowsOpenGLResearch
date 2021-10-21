#pragma once

#include <game_engine/scene_manager.h>
#include <game_engine/time_manager.h>
#include <game_engine/rendering_window_info_provider.h>
#include <game_engine/read_only_fs_abstraction.h>
#include <game_engine/fs_abstraction.h>
#include <game_engine/mesh_loader.h>
#include <game_engine/mesh_renderer_factory.h>
#include <game_engine/mesh_storage.h>
#include <game_engine/units_converter.h>
#include <memory>

namespace GameEngine
{
class ServiceLocator
{
public:
    void provide(std::shared_ptr<SceneManager> sceneManager);
    void provide(std::shared_ptr<TimeManager> timeManager);
    void provide(std::shared_ptr<RenderingWindowInfoProvider> renderingWindowInfoProvider);
    void provide(std::shared_ptr<FsAbstraction> fsAbstraction);
    void provide(std::shared_ptr<ReadOnlyFsAbstraction> readOnlyFsAbstraction);
    void provide(std::shared_ptr<MeshLoader> meshLoader);
    void provide(std::shared_ptr<MeshStorage> meshStorage);
    void provide(std::shared_ptr<MeshRendererFactory> meshRendererFactory);
    void provide(std::shared_ptr<UnitsConverter> unitsConverter);

    SceneManager* sceneManager();
    TimeManager* timeManager();
    RenderingWindowInfoProvider* renderingWindowInfoProvider();
    FsAbstraction* fsAbstraction();
    ReadOnlyFsAbstraction* readOnlyFsAbstraction();
    MeshLoader* meshLoader();
    MeshStorage* meshStorage();
    MeshRendererFactory* meshRendererFactory();
    UnitsConverter* unitsConverter();

private:
    std::shared_ptr<SceneManager> m_sceneManager;
    std::shared_ptr<TimeManager> m_timeManager;
    std::shared_ptr<RenderingWindowInfoProvider> m_renderingWindowInfoProvider;
    std::shared_ptr<FsAbstraction> m_fsAbstraction;
    std::shared_ptr<ReadOnlyFsAbstraction> m_readOnlyFsAbstraction;
    std::shared_ptr<MeshLoader> m_meshLoader;
    std::shared_ptr<MeshStorage> m_meshStorage;
    std::shared_ptr<MeshRendererFactory> m_meshRendererFactory;
    std::shared_ptr<UnitsConverter> m_unitsConverter;
};
}
