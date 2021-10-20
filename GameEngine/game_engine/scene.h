#pragma once

#include <memory>

namespace GameEngine
{
class ServiceLocator;

class Scene
{
public:
    explicit Scene(std::shared_ptr<ServiceLocator> serviceLocator) : m_serviceLocator(std::move(serviceLocator)) {}

    virtual ~Scene() = default;

    virtual void update();

    /*std::shared_ptr<GameObject> rootGameObject() { return m_rootGameObject; }

    virtual void buildHierarchyFromRepresentation(const std::string& hierarchyRepresentation);*/
protected:
    std::shared_ptr<ServiceLocator> m_serviceLocator;
};
}
