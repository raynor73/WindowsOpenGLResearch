#include "rendering_window_info_update_detector.h"
#include <game_engine/service_locator.h>
#include <cmath>

using namespace GameEngine;
using namespace std;

bool RenderingWindowInfoUpdateDetector::isDisplayInfoUpdated()
{
    auto renderingWindowInfoProvider = m_serviceLocator->renderingWindowInfoProvider();

    auto currentDisplayWidth = renderingWindowInfoProvider->width();
    auto currentDisplayHeight = renderingWindowInfoProvider->height();
    auto currentDisplayDensityFactor = renderingWindowInfoProvider->densityFactor();

    auto isDisplayInfoUpdated =
        isnan(m_lastDisplayWidth) ||
        isnan(m_lastDisplayHeight) ||
        isnan(m_lastDisplayDensityFactor) ||
        abs(currentDisplayWidth - m_lastDisplayWidth) > FLT_EPSILON ||
        abs(currentDisplayHeight - m_lastDisplayHeight) > FLT_EPSILON ||
        abs(currentDisplayDensityFactor - m_lastDisplayDensityFactor) > FLT_EPSILON;

    m_lastDisplayWidth = currentDisplayWidth;
    m_lastDisplayHeight = currentDisplayHeight;
    m_lastDisplayDensityFactor = currentDisplayDensityFactor;

    return isDisplayInfoUpdated;
}
