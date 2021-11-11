#pragma once

#include <game_engine/game_object_component.h>
#include <memory>

namespace GameEngine
{
class PhysicsEngine;

class RigidBodyComponent : public GameObjectComponent
{
protected:
    std::weak_ptr<PhysicsEngine> m_physicsEngine;

public:
    RigidBodyComponent(std::weak_ptr<PhysicsEngine> physicsEngine) : m_physicsEngine(physicsEngine) {}

    virtual ~RigidBodyComponent() override;

    virtual void onGameObjectAttachedToParent() override;
    virtual void onGameObjectDetachedFromParent() override;

    virtual void update() override;

    virtual void setEnabled(bool isEnabled) override;

    bool isKinematic();
};
}
