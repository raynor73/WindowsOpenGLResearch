#pragma once

namespace GameEngine
{
class MouseInput
{
public:
    virtual ~MouseInput() = default;

    virtual float dx() = 0;
    virtual float dy() = 0;
};
}
