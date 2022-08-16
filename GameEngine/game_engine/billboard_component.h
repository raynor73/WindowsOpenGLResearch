#pragma once

#include <game_engine/game_object_component.h>

namespace GameEngine 
{
class BillboardComponent : public GameObjectComponent
{
public:
    static const std::string TYPE_NAME;

    virtual void update() override;

    virtual const std::string& typeName() const override { return TYPE_NAME; }

    virtual std::shared_ptr<GameObjectComponent> clone() override;
};
}
