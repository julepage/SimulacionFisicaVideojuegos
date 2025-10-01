#pragma once
#include "Vector3D.h"
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"

class Particula
{
public:
	Particula(Vector3D pos, Vector3D vel, float acc, float d);
	virtual ~Particula() { DeregisterRenderItem(renderItem); renderItem = nullptr; };
	
	void integrate(double t);

private:
	Vector3D velocidad;
	physx::PxTransform pose;
	RenderItem* renderItem;
	float ac;
	float damping;
};

