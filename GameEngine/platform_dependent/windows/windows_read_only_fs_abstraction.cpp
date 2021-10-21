#include "windows_read_only_fs_abstraction.h"

#include <fstream>
#include <sstream>
#include <filesystem>

using namespace GameEngine;
using namespace std;

bool WindowsReadOnlyFsAbstraction::isFileExists(const string& path)
{
    ifstream f(path.c_str());
    return f.good();
}

vector<uint8_t> WindowsReadOnlyFsAbstraction::readBinaryFileContent(const string& path)
{
    vector<uint8_t> buffer;
    
    ifstream f(path.c_str(), ios::in | ios::binary);
    f.read(reinterpret_cast<char *>(buffer.data()), filesystem::file_size(path));
    f.close();

    return buffer;
}

string WindowsReadOnlyFsAbstraction::readTextFileContent(const string& path)
{
    stringstream ss;

    ifstream f(path.c_str(), ios::in);
    ss << f.rdbuf();
    f.close();

    return ss.str();
}
