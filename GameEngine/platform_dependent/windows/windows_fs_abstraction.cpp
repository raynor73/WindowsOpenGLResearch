#include "windows_fs_abstraction.h"
#include <game_engine/service_locator.h>
#include <filesystem>
#include <fstream>

using namespace GameEngine;
using namespace std;

bool WindowsFsAbstraction::isFileExists(const string& path)
{
    return m_serviceLocator->readOnlyFsAbstraction()->isFileExists(path);
}

vector<uint8_t> WindowsFsAbstraction::readBinaryFileContent(const string& path)
{
    return m_serviceLocator->readOnlyFsAbstraction()->readBinaryFileContent(path);
}

void WindowsFsAbstraction::writeBinaryFileContent(const string& path, const vector<uint8_t>& data)
{
    ofstream f(path.c_str(), ios::out | ios::binary);
    f.write(reinterpret_cast<const char *>(data.data()), data.size());
    f.close();
}

string WindowsFsAbstraction::readTextFileContent(const string& path)
{
    return m_serviceLocator->readOnlyFsAbstraction()->readTextFileContent(path);
}

void WindowsFsAbstraction::writeTextFileContent(const string& path, const string& text)
{
    ofstream f(path.c_str(), ios::out);
    f << text;
    f.close();
}

void WindowsFsAbstraction::deleteFile(const string& path)
{
    filesystem::remove(path);
}
