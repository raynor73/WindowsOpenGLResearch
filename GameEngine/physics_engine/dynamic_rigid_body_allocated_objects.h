#pragma once

#include <bullet/btBulletDynamicsCommon.h>

namespace GameEngine
{
struct DynamicRigidBodyAllocatedObjects
{
    btDefaultMotionState* btMotionState;
    btRigidBody* btRigidBody;
};
}
