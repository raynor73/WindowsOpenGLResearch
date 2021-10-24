#pragma once

namespace GameEngine
{
class KeyboardInput
{
public:
    static const int KEY_ESC = 4;

    static const int KEY_W = 0;
    static const int KEY_S = 1;
    static const int KEY_A = 2;
    static const int KEY_D = 3;

    virtual ~KeyboardInput() = default;

    virtual bool isKeyPressed(int key) = 0;
};
}
