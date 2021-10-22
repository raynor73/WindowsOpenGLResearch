#include "camera_component.h"

using namespace GameEngine;
using namespace std;

void CameraComponent::setLayerNames(const vector<string>& layerNames)
{
    m_layerNames.clear();
    m_layerNames = layerNames;
}
