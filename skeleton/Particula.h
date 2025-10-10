#pragma once
#include "Vector3D.h"
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"

class Particula
{
public:
	Particula(Vector3D pos, Vector3D vel, Vector3D acc, float d, float masa = 1.0, float g = 9.8);
	virtual ~Particula() { DeregisterRenderItem(renderItem); renderItem = nullptr; };
	
	virtual void integrate(double t);

	Vector3D getVel() { return velocidad; };
	void setAc(Vector3D Acc) { ac = Acc; }

protected:
	Vector3D velocidad;
	physx::PxTransform pose;
	RenderItem* renderItem;
	Vector3D ac;
	float damping;
	float masa;
	float gravedad;
};

