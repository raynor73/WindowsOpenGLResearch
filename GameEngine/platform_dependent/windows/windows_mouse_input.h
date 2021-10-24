#pragma once

#include <cmath>
#include <game_engine/input/mouse_input.h>
#include <game_engine/without_generated_methods.h>
#include <GLFW/glfw3.h>

namespace GameEngine 
{
class WindowsMouseInput : public MouseInput, public WithoutGeneratedMethods
{
    float m_prevXPosition = NAN;
    float m_prevYPosition = NAN;

    float m_dx = 0;
    float m_dy = 0;

    GLFWwindow* m_window;

public:
    explicit WindowsMouseInput(GLFWwindow* window) : m_window(window) {}

    virtual float dx() override { return m_dx; }
    virtual float dy() override { return m_dy; }

    void update();
};
}
