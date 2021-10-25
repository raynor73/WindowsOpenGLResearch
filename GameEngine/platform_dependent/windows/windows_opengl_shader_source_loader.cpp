#include "windows_opengl_shader_source_loader.h"
#include <game_engine/service_locator.h>

using namespace GameEngine;
using namespace std;

string WindowsOpenGLShaderSourceLoader::loadShaderSource(const string& path)
{
    return m_serviceLocator->readOnlyFsAbstraction()->readTextFileContent(path);
}
