#include "service_locator.h"
#include <stdexcept>

using namespace GameEngine;
using namespace std;

void ServiceLocator::provide(shared_ptr<SceneManager> sceneManager)
{
    if (m_sceneManager.use_count() > 0) {
        throw domain_error("Scene Manager already provided");
    }

    m_sceneManager = sceneManager;
}

void ServiceLocator::provide(shared_ptr<TimeManager> timeManager)
{
    if (m_timeManager.use_count() > 0) {
        throw domain_error("Time Manager already provided");
    }

    m_timeManager = timeManager;
}

SceneManager* ServiceLocator::sceneManager()
{
    if (m_sceneManager.use_count() == 0) {
        throw domain_error("Scene Manager is not provided");
    }

    return m_sceneManager.get();
}

TimeManager* GameEngine::ServiceLocator::timeManager()
{
    if (m_timeManager.use_count() == 0) {
        throw domain_error("Time Manager is not provided");
    }

    return m_timeManager.get();
}
