#pragma once

#include <physics_engine/dynamic_rigid_body_allocated_objects.h>

namespace GameEngine
{
struct CylinderRigidBodyAllocatedObjects : public DynamicRigidBodyAllocatedObjects
{
    btCylinderShape* btCylinderShape;
};
}

