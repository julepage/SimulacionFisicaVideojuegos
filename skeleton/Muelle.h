#pragma once
#include "GeneradorFuerzas.h"
using namespace physx;

class Muelle: public GeneradorFuerzas
{
public:
	Muelle( Vector3D pos, float k, float longitud): pos(pos), k(k), longitud(longitud) {
		//Creo un cuadrado en la pos un poco mas atrás como ancla
		ancla = new RenderItem(CreateShape(PxBoxGeometry(1.0f, 1.0f, 1.0f)), new PxTransform(PxVec3(pos.getX(), pos.getY(), pos.getZ())), Vector4(1.0f, 0.0f, 0.0f, 1.0f));
		activa = true;
	};
	virtual ~Muelle() { if (ancla != nullptr) { DeregisterRenderItem(ancla);  ancla = nullptr; } };
	void updateFuerzas(Particula* p, double t) override; // por defecto no hace nada pero es por si necesito actualizar por frmae y no por particulas como en la explosion
	void toggle() override;

protected:
	float k;//cte elastica muelle
	Vector3D pos;//posicion inicial
	float longitud;//longitud del objeto
	RenderItem* ancla;
};

