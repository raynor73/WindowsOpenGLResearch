#pragma once

#include <memory>
#include <unordered_set>
#include <game_engine/collisions_info_component.h>
#include <game_engine/without_generated_methods.h>

namespace GameEngine
{
class CollisionsInfoComponentsManager : public WithoutGeneratedMethods
{
    std::unordered_set<std::shared_ptr<CollisionsInfoComponent>> m_collisionsInfoComponents;

public:
    virtual ~CollisionsInfoComponentsManager() = default;

    virtual const std::unordered_set<std::shared_ptr<CollisionsInfoComponent>>& collisionsInfoComponents();

    virtual std::shared_ptr<CollisionsInfoComponent> createCollisionsInfoComponent();
    virtual void releaseCollisionsInfoComponent(std::shared_ptr<CollisionsInfoComponent> collisionsInfoComponent);

    virtual void reset();
};
}
