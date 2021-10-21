#pragma once

#include <string>
#include <glm/glm.hpp>

namespace GameEngine
{
struct Material {
    glm::vec4 diffuseColor;
    glm::vec4 topColor;
    glm::vec4 bottomColor;
    std::string textureName;
    bool useDiffuseColor;
    bool isTranslucent;
    bool isWireframe;
    bool isUnlit;
    bool isGradient;
    bool isDoubleSided;
};
}
