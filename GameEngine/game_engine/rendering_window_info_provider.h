#pragma once

namespace GameEngine
{
class RenderingWindowInfoProvider
{
public:
    virtual float width() const = 0;
    virtual float height() const = 0;
    virtual float densityFactor() const = 0;
};
}
