#pragma once
#include "Particula.h"


class Proyectil : public Particula
{
public:

	Proyectil(Vector3D pos, Vector3D velDir, float vR, float vS, float masaR, Vector4 color = Vector4(1.0f, 1.0f, 0.0f, 1.0f), physx::PxGeometry* forma = nullptr, float d = 1.0, float gR = 9.8);

	virtual ~Proyectil() {};

protected:

	float calcularMasaSimulada(float masaR, float velR, float velS);
	float gravedadSimulada(float gR, float velR, float velS);
	float velocidad;
};

