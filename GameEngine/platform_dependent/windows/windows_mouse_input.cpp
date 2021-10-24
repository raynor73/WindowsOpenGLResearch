#include "windows_mouse_input.h"
#include <cmath>
#include <game_engine/logger.h>
#include <sstream>

using namespace GameEngine;
using namespace std;

void WindowsMouseInput::update()
{
    double currentXPosition, currentYPosition;
    glfwGetCursorPos(m_window, &currentXPosition, &currentYPosition);

    if (!isnan(m_prevXPosition)) {
        m_dx = float(currentXPosition) - m_prevXPosition;
    }
    m_prevXPosition = float(currentXPosition);

    if (!isnan(m_prevYPosition)) {
        m_dy = float(currentYPosition) - m_prevYPosition;
    }
    m_prevYPosition = float(currentYPosition);
}
