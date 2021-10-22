#pragma once

#include <utility>
#include <string>
#include <game_engine/mesh.h>
#include <game_engine/game_object_component.h>

namespace GameEngine
{
class MeshComponent : public GameObjectComponent
{
    Mesh m_mesh;
    std::string m_meshName;

public:
    static const std::string TYPE_NAME;

    MeshComponent(Mesh mesh, std::string meshName) : m_mesh(std::move(mesh)), m_meshName(std::move(meshName)) {}

    const Mesh& mesh() const { return m_mesh; }

    const std::string& meshName() const { return m_meshName; }

    virtual const std::string& typeName() const override { return TYPE_NAME; }
    virtual std::shared_ptr<GameObjectComponent> clone() override;
};
}
