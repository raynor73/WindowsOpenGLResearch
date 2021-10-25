#include "collisions_info_component.h"

std::shared_ptr<GameObjectComponent> GameEngine::CollisionsInfoComponent::clone()
{
    return std::shared_ptr<GameObjectComponent>();
}
