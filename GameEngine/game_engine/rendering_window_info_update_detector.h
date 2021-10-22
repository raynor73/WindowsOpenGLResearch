#pragma once

#include <memory>

namespace GameEngine
{
class ServiceLocator;

class RenderingWindowInfoUpdateDetector
{
    std::shared_ptr<ServiceLocator> m_serviceLocator;
    
    float m_lastDisplayWidth = NAN;
    float m_lastDisplayHeight = NAN;
    float m_lastDisplayDensityFactor = NAN;

public:
    explicit RenderingWindowInfoUpdateDetector(std::shared_ptr<ServiceLocator> serviceLocator) : m_serviceLocator(serviceLocator) {}

    bool isDisplayInfoUpdated();
};
}
