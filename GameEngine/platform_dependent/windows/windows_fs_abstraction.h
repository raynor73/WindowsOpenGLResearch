#pragma once

#include <memory>
#include <game_engine/fs_abstraction.h>
#include <game_engine/without_generated_methods.h>

namespace GameEngine
{
class ServiceLocator;

class WindowsFsAbstraction : public FsAbstraction, public WithoutGeneratedMethods
{
    std::shared_ptr<ServiceLocator> m_serviceLocator;

public:
    explicit WindowsFsAbstraction(std::shared_ptr<ServiceLocator> serviceLocator) : m_serviceLocator(serviceLocator) {}

    virtual bool isFileExists(const std::string& path);

    virtual std::vector<uint8_t> readBinaryFileContent(const std::string& path);
    virtual void writeBinaryFileContent(const std::string& path, const std::vector<uint8_t>& data);

    virtual std::string readTextFileContent(const std::string& path);
    virtual void writeTextFileContent(const std::string& path, const std::string& text);

    virtual void deleteFile(const std::string& path);
};
}
