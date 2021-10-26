#include "collisions_info_component.h"

using namespace GameEngine;
using namespace std;

shared_ptr<GameObjectComponent> GameEngine::CollisionsInfoComponent::clone()
{
    return shared_ptr<GameObjectComponent>();
}
