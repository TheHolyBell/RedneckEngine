#pragma once

#include <btBulletDynamicsCommon.h>

class PhysicsWorld
{
public:
	static void Initialize();
	static void Update(float dt);

private:
	static btDynamicsWorld* 
};