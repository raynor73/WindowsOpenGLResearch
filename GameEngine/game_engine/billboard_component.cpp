#include "billboard_component.h"
#include <game_engine/game_object.h>
#include <game_engine/transformation_component.h>
#include <game_engine/utils.h>
#include <sstream>

using namespace GameEngine;
using namespace std;

const string BillboardComponent::TYPE_NAME = "BillboardComponent";

void BillboardComponent::update()
{
    throwErrorIfNoGameObject();
    auto gameObject = m_gameObject.lock();
    auto transformation = gameObject->findComponent<TransformationComponent>();
    Utils::throwErrorIfNull(transformation, [&]() {
        stringstream ss;
        ss << "Transformation Component not found for Game Object " << gameObject->name() << " while updating Billboard orientation";
        return ss.str();
    });


}

shared_ptr<GameObjectComponent> BillboardComponent::clone()
{
    auto clone = make_shared<BillboardComponent>();
    clone->setEnabled(m_isEnabled);
    return clone;
}
