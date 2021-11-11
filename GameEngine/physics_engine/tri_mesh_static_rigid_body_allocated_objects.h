#pragma once

#include <physics_engine/static_rigid_body_allocated_objects.h>

namespace GameEngine
{
struct TriMeshStaticRigidBodyAllocatedObjects : public StaticRigidBodyAllocatedObjects
{
    btTriangleMesh* btMesh;
    btBvhTriangleMeshShape* btCollisionShape;
};
}
