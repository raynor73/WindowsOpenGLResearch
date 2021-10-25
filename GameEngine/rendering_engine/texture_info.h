#pragma once

#include <GL/glew.h>
#include <cstdint>

namespace GameEngine
{
struct TextureInfo {
    GLuint texture;
    uint32_t width;
    uint32_t height;
};
}
