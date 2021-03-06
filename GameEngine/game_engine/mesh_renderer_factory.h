#pragma once

#include <memory>
#include <vector>
#include <string>
#include <game_engine/game_object_component.h>

namespace GameEngine
{
class MeshRendererFactory
{
public:
    virtual ~MeshRendererFactory() = default;

    virtual std::shared_ptr<GameObjectComponent> createMeshRenderer(std::vector<std::string> layerNames) = 0;
    virtual void releaseMeshRenderer(std::shared_ptr<GameObjectComponent> meshRenderer) = 0;

    virtual void reset() = 0;
};
}
