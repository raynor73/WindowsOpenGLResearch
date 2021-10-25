#pragma once

#include <vector>
#include <game_engine/collision_info.h>
#include <game_engine/game_object_component.h>

namespace GameEngine
{
class CollisionsInfoComponent : public GameObjectComponent
{
    std::vector<CollisionInfo> m_collisions;

public:
    static const std::string TYPE_NAME;

    void add(const CollisionInfo& collisionInfo);
    //void
    void removeAllCollisionsInfo();

    virtual const std::string& typeName() const override { return TYPE_NAME; }

    virtual std::shared_ptr<GameObjectComponent> clone() override;
};
}
