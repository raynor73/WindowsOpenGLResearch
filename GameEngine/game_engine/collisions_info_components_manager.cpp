#include "collisions_info_components_manager.h"

using namespace GameEngine;
using namespace std;

const unordered_set<shared_ptr<CollisionsInfoComponent>>& CollisionsInfoComponentsManager::collisionsInfoComponents()
{
    return m_collisionsInfoComponents;
}

shared_ptr<CollisionsInfoComponent> CollisionsInfoComponentsManager::createCollisionsInfoComponent()
{
    auto collisionsInfoComponent = shared_ptr<CollisionsInfoComponent>(new CollisionsInfoComponent());
    m_collisionsInfoComponents.insert(collisionsInfoComponent);
    return collisionsInfoComponent;
}

void CollisionsInfoComponentsManager::releaseCollisionsInfoComponent(shared_ptr<CollisionsInfoComponent> collisionsInfoComponent)
{
    m_collisionsInfoComponents.erase(collisionsInfoComponent);
}

void CollisionsInfoComponentsManager::reset()
{
    m_collisionsInfoComponents.clear();
}
