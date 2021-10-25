#pragma once

#include <GL/glew.h>

namespace GameEngine
{
struct IboInfo {
    GLuint ibo;
    size_t numberOfIndices;
};
}
