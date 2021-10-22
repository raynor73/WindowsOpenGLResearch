#pragma once

#include <memory>
#include <game_engine/bitmap_loader.h>
#include <game_engine/without_generated_methods.h>

namespace GameEngine
{
class ServiceLocator;

class WindowsBitmapLoader : public BitmapLoader, public WithoutGeneratedMethods
{
    std::shared_ptr<ServiceLocator> m_serviceLocator;

public:
    explicit WindowsBitmapLoader(std::shared_ptr<ServiceLocator> serviceLocator) : m_serviceLocator(serviceLocator) {}

    virtual BitmapInfo loadBitmap(const std::string& path) override;
};
}
