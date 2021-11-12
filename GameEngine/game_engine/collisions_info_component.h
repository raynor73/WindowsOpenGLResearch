#pragma once

#include <vector>
#include <game_engine/collision_info.h>
#include <game_engine/game_object_component.h>

namespace GameEngine
{
class CollisionsInfoComponentsManager;

class CollisionsInfoComponent : public GameObjectComponent
{
    std::vector<CollisionInfo> m_collisions;

    friend class CollisionsInfoComponentsManager;

public:
    static const std::string TYPE_NAME;

    const std::vector<CollisionInfo>& collisions() { return m_collisions; }

    void addCollisionInfo(const CollisionInfo& collisionInfo);

    void removeAllCollisionsInfo();

    virtual const std::string& typeName() const override { return TYPE_NAME; }

    virtual std::shared_ptr<GameObjectComponent> clone() override;

private:
    CollisionsInfoComponent() = default;
};
}
