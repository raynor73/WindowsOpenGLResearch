#pragma once

#include <cstdio>
#include <ios>
#include <iostream>
#include <Windows.h>

namespace GameEngine
{
namespace Windows
{
namespace Utils
{
    bool redirectConsoleIO();
    bool releaseConsole();
    void adjustConsoleBuffer(int16_t minLength);
    bool createNewConsole(int16_t minLength);
    bool attachParentConsole(int16_t minLength);
}
}
}
