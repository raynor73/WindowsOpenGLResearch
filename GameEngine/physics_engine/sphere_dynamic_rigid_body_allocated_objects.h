#pragma once

#include <physics_engine/dynamic_rigid_body_allocated_objects.h>

namespace GameEngine
{
struct SphereDynamicRigidBodyAllocatedObjects : public DynamicRigidBodyAllocatedObjects
{
    btSphereShape* btSphereShape;
};
}

