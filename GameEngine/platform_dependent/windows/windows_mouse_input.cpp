#include "windows_mouse_input.h"
#include <cmath>

using namespace GameEngine;
using namespace std;

void WindowsMouseInput::update(float currentXPosition, float currentYPosition)
{
    if (!isnan(m_prevXPosition)) {
        m_dx = currentXPosition - m_prevXPosition;
    }
    m_prevXPosition = currentXPosition;

    if (!isnan(m_prevYPosition)) {
        m_dy = currentYPosition - m_prevYPosition;
    }
    m_prevYPosition = currentYPosition;
}
