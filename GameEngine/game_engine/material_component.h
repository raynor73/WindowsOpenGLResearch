#pragma once

#include <string>
#include <utility>
#include <game_engine/material.h>
#include <game_engine/game_object_component.h>

namespace GameEngine
{
class MaterialComponent : public GameObjectComponent {

    Material m_material;

public:
    static const std::string TYPE_NAME;

    MaterialComponent(Material material) : m_material(std::move(material)) {}

    const Material& material() const { return m_material; }
    void setMaterial(const Material& material) { m_material = material; }

    virtual const std::string& typeName() const override { return TYPE_NAME; };
    virtual std::shared_ptr<GameObjectComponent> clone() override;
};
}
