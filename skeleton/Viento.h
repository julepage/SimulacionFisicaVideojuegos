#pragma once
#include "GeneradorFuerzas.h"

class Viento : public GeneradorFuerzas
{
public:
	virtual ~Viento() = default;
	Viento(float velocidad, Vector3D direccion, float c) : vel(direccion.normalize()* velocidad), k1(c)
	{
	}
	void updateFuerzas(Particula* p, double t = 0.0) override;
protected:
	Vector3D vel;
	float k1; // coeficiente de rozamiento con el aire
	Vector3D fuerzaViento;
	float k2 = 0;
};

