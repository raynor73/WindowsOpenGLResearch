#include "windows_keyboard_input.h"

bool GameEngine::WindowsKeyboardInput::isKeyPressed(int key)
{
    return m_pressedKeys.count(key) != 0;
}

void GameEngine::WindowsKeyboardInput::setKeyPressed(int key, bool isPressed)
{
    if (isPressed) {
        m_pressedKeys.insert(key);
    }
    else {
        m_pressedKeys.erase(key);
    }
}
