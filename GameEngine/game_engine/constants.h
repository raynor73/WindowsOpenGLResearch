#pragma once

#include <string>
#include <glm/glm.hpp>

namespace GameEngine 
{
namespace Constants
{
const std::string LOG_TAG = "GameEngine";

const glm::vec3 DEFAULT_FORWARD_DIRECTION(0, 0, -1);
const glm::vec3 CAMERA_UP_DIRECTION(0, 1, 0);
constexpr int MAX_JOINTS = 64;
}
}
