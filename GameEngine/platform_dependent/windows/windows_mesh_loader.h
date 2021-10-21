#pragma once

#include <game_engine/service_locator.h>
#include <game_engine/without_generated_methods.h>


namespace GameEngine
{
class WindowsMeshLoader : public MeshLoader, public WithoutGeneratedMethods
{
    std::shared_ptr<ServiceLocator> m_serviceLocator;

public:
    WindowsMeshLoader(std::shared_ptr<ServiceLocator> serviceLocator) : m_serviceLocator(serviceLocator) {}

    virtual Mesh loadMesh(const std::string& path) override;
};
}
