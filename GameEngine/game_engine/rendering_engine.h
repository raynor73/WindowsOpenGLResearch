#pragma once

namespace GameEngine
{
class RenderingEngine
{
public:
    virtual ~RenderingEngine() = default;

    virtual void reset() = 0;
};
}
