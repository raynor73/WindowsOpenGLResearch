#pragma once

#include <memory>

namespace GameEngine 
{
class ServiceLocator;

class TimeManager
{
    std::shared_ptr<ServiceLocator> m_serviceLocator;

    float m_timeScale = 1;

    float m_deltaTime = 0;
    float m_unscaledDeltaTime = 0;

    uint64_t m_prevTimestamp = 0;
    bool m_hasPrevTimestamp = false;

public:
    explicit TimeManager(std::shared_ptr<ServiceLocator> serviceLocator) : m_serviceLocator(std::move(serviceLocator)) {}

    void update();

    float timeScale() const { return m_timeScale; }
    void setTimeScale(float scale) { m_timeScale = scale; }

    float deltaTime() const { return m_deltaTime; }

    float unscaledDeltaTime() const { return m_unscaledDeltaTime; }
};
}
