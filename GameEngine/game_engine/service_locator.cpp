#include "service_locator.h"
#include <stdexcept>

using namespace GameEngine;
using namespace std;

void ServiceLocator::provide(shared_ptr<SceneManager> sceneManager)
{
    if (m_sceneManager.use_count() > 0) {
        throw domain_error("Scene Manager already provided");
    }

    m_sceneManager = sceneManager;
}

void ServiceLocator::provide(shared_ptr<TimeProvider> timeProvider)
{
    if (m_timeProvider.use_count() > 0) {
        throw domain_error("Time Provider already provided");
    }

    m_timeProvider = timeProvider;
}

void ServiceLocator::provide(shared_ptr<TimeManager> timeManager)
{
    if (m_timeManager.use_count() > 0) {
        throw domain_error("Time Manager already provided");
    }

    m_timeManager = timeManager;
}

void ServiceLocator::provide(shared_ptr<RenderingWindowInfoProvider> renderingWindowInfoProvider)
{
    if (m_renderingWindowInfoProvider.use_count() > 0) {
        throw domain_error("Rendering Window Info Provider already provided");
    }

    m_renderingWindowInfoProvider = renderingWindowInfoProvider;
}

void ServiceLocator::provide(shared_ptr<FsAbstraction> fsAbstraction)
{
    if (m_fsAbstraction.use_count() > 0) {
        throw domain_error("FS Abstraction already provided");
    }

    m_fsAbstraction = fsAbstraction;
}

void ServiceLocator::provide(shared_ptr<ReadOnlyFsAbstraction> readOnlyFsAbstraction)
{
    if (m_readOnlyFsAbstraction.use_count() > 0) {
        throw domain_error("Read Only FS Abstraction already provided");
    }

    m_readOnlyFsAbstraction = readOnlyFsAbstraction;
}

void ServiceLocator::provide(shared_ptr<MeshLoader> meshLoader)
{
    if (m_meshLoader.use_count() > 0) {
        throw domain_error("Mesh Loader already provided");
    }

    m_meshLoader = meshLoader;
}

void ServiceLocator::provide(shared_ptr<MeshRendererFactory> meshRendererFactory)
{
    if (m_meshRendererFactory.use_count() > 0) {
        throw domain_error("Mesh Renderer Factory already provided");
    }

    m_meshRendererFactory = meshRendererFactory;
}

void ServiceLocator::provide(shared_ptr<UnitsConverter> unitsConverter)
{
    if (m_unitsConverter.use_count() > 0) {
        throw domain_error("Units Converter already provided");
    }

    m_unitsConverter = unitsConverter;
}

void ServiceLocator::provide(shared_ptr<SceneHierarchyLoader> sceneHierarchyLoader)
{
    if (m_sceneHierarchyLoader != nullptr) {
        throw domain_error("Scene Hierarchy Loader already provided");
    }

    m_sceneHierarchyLoader = sceneHierarchyLoader;
}

SceneManager* ServiceLocator::sceneManager()
{
    if (m_sceneManager.use_count() == 0) {
        throw domain_error("Scene Manager is not provided");
    }

    return m_sceneManager.get();
}

TimeProvider* ServiceLocator::timeProvider()
{
    if (m_timeProvider.use_count() == 0) {
        throw domain_error("Time Provider is not provided");
    }

    return m_timeProvider.get();
}

TimeManager* GameEngine::ServiceLocator::timeManager()
{
    if (m_timeManager.use_count() == 0) {
        throw domain_error("Time Manager is not provided");
    }

    return m_timeManager.get();
}

RenderingWindowInfoProvider* GameEngine::ServiceLocator::renderingWindowInfoProvider()
{
    if (m_renderingWindowInfoProvider.use_count() == 0) {
        throw domain_error("Rendering Window Info Provider is not provided");
    }

    return m_renderingWindowInfoProvider.get();
}

FsAbstraction* ServiceLocator::fsAbstraction()
{
    if (m_fsAbstraction.use_count() == 0) {
        throw domain_error("FS Abstraction is not provided");
    }

    return m_fsAbstraction.get();
}

ReadOnlyFsAbstraction* ServiceLocator::readOnlyFsAbstraction()
{
    if (m_readOnlyFsAbstraction.use_count() == 0) {
        throw domain_error("Read-only FS Abstraction is not provided");
    }

    return m_readOnlyFsAbstraction.get();
}

MeshLoader* ServiceLocator::meshLoader()
{
    if (m_meshLoader.use_count() == 0) {
        throw domain_error("Mesh Loader is not provided");
    }

    return m_meshLoader.get();
}

MeshRendererFactory* ServiceLocator::meshRendererFactory()
{
    if (m_meshRendererFactory.use_count() == 0) {
        throw domain_error("Mesh Renderer Factory is not provided");
    }

    return m_meshRendererFactory.get();
}

UnitsConverter* ServiceLocator::unitsConverter()
{
    if (m_unitsConverter.use_count() == 0) {
        throw domain_error("Units Converter is not provided");
    }

    return m_unitsConverter.get();
}

SceneHierarchyLoader* GameEngine::ServiceLocator::sceneHierarchyLoader()
{
    if (m_sceneHierarchyLoader == nullptr) {
        throw domain_error("Scene Hierarchy Loader is not provided");
    }

    return m_sceneHierarchyLoader.get();
}
