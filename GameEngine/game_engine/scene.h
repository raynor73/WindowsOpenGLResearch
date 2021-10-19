#pragma once

namespace GameEngine
{
class Scene
{
public:
    virtual ~Scene() = default;

    virtual void update();

    /*std::shared_ptr<GameObject> rootGameObject() { return m_rootGameObject; }

    virtual void buildHierarchyFromRepresentation(const std::string& hierarchyRepresentation);

    virtual void update();*/
};
}
