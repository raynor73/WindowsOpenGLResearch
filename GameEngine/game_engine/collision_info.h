#pragma once

#include <vector>
#include <memory>
#include <glm/vec3.hpp>
#include <game_engine/game_object.h>

namespace GameEngine
{
struct CollisionInfo {
    std::weak_ptr<GameObject> gameObject;
    glm::vec3 position;
    glm::vec3 normal;
    float depth;
};
}
