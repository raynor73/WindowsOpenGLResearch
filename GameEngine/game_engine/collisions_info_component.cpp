#include "collisions_info_component.h"

using namespace GameEngine;
using namespace std;

const string CollisionsInfoComponent::TYPE_NAME = "CollisionsInfoComponent";

void CollisionsInfoComponent::addCollisionInfo(const CollisionInfo& collisionInfo)
{
    m_collisions.push_back(collisionInfo);
}

void CollisionsInfoComponent::removeAllCollisionsInfo()
{
    m_collisions.clear();
}

shared_ptr<GameObjectComponent> CollisionsInfoComponent::clone()
{
    auto clone = new CollisionsInfoComponent();
    clone->setEnabled(m_isEnabled);
    return std::shared_ptr<CollisionsInfoComponent>(clone);
}
