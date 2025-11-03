#pragma once
#include "Vector3D.h"
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"

class Particula
{
public:
	Particula(Vector3D pos, Vector3D vel, Vector3D acc, float d = 1.0, float masa = 1.0, float g = 9.8, float vidas = 6.0f);
	virtual ~Particula() { DeregisterRenderItem(renderItem); renderItem = nullptr; };

	virtual void integrate(double t);

	Vector3D getVel() { return velocidad; };
	Vector3D getPos() { return Vector3D(pose.p.x, pose.p.y, pose.p.z); };
	//getter
	float getVidas() const { return vida; };
	float getMasa() const { return masa; };
	Vector3D getPosIni() const { return posIni; };
	//setter
	void setVidas(float v) { vida = v; }
	void setAc(Vector3D Acc) { ac = Acc; }
	void addFuerza(const Vector3D& f) { fuerzaAcumulada = fuerzaAcumulada + f; };

protected:
	Vector3D velocidad;
	physx::PxTransform pose;
	RenderItem* renderItem;
	Vector3D ac;
	Vector3D posIni;
	float damping;
	float masa;
	float gravedad;
	float vida;//Segumdo de vida

	Vector3D fuerzaAcumulada;
};

