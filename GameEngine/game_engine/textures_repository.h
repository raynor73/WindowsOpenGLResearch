#pragma once

#include <string>
#include <vector>

namespace GameEngine
{
class TexturesRepository
{
public:
    virtual void createTexture(const std::string& name, uint32_t width, uint32_t height, const std::vector<uint8_t>& data) = 0;
    virtual void createDisplayDensityFactorAwareTexture(const std::string& name, const std::string& path) = 0;
    virtual void createTexture(const std::string& name, const std::string& path) = 0;
    virtual void removeTexture(const std::string& name) = 0;
    virtual void removeAllTextures() = 0;
};
}