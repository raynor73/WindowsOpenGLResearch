#pragma once

#include <game_engine/input/keyboard_input.h>
#include <game_engine/without_generated_methods.h>
#include <GLFW/glfw3.h>
#include <unordered_set>

namespace GameEngine
{
class WindowsKeyboardInput : public KeyboardInput, public WithoutGeneratedMethods
{
    std::unordered_set<int> m_pressedKeys;

public:
    virtual bool isKeyPressed(int key) override;

    void setKeyPressed(int key, bool isPressed);
};
}
