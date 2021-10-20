#include "time_manager.h"

using namespace GameEngine;
using namespace std;

void TimeManager::update()
{
    auto currentTimestamp = m_timeProvider->getTimestamp();

    if (m_hasPrevTimestamp) {
        m_unscaledDeltaTime = (currentTimestamp - m_prevTimestamp) / TimeProvider::NANOS_IN_SECOND;
    } else {
        m_unscaledDeltaTime = 0.0f;
    }
    m_deltaTime = m_unscaledDeltaTime * m_timeScale;

    m_prevTimestamp = currentTimestamp;
    m_hasPrevTimestamp = true;
}
