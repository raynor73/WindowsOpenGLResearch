#pragma once

#include <memory>
#include <unordered_map>
#include <game_engine/game_object.h>

namespace GameEngine
{
class ServiceLocator;

class Scene
{
public:
    explicit Scene(std::shared_ptr<ServiceLocator> serviceLocator);

    virtual ~Scene() = default;

    virtual void update();

    std::shared_ptr<GameObject> rootGameObject() { return m_rootGameObject; }

    void addGameObject(const std::string& parentName, const std::shared_ptr<GameObject>& gameObject);
    void removeGameObject(const std::string& name);

    //virtual void buildHierarchyFromRepresentation(const std::string& hierarchyRepresentation);*/

    static const std::string ROOT_GAME_OBJECT_NAME;
protected:
    std::shared_ptr<ServiceLocator> m_serviceLocator;
    std::shared_ptr<GameObject> m_rootGameObject;
    std::unordered_map<std::string, std::shared_ptr<GameObject>> m_gameObjectsMap;
};
}
