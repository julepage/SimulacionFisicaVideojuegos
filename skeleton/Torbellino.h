#pragma once
#include "Viento.h"

class Torbellino : public Viento
{
public:
	Torbellino(Vector3D c, float intensidad, float r) : centro(c),K(intensidad), radio(r), Viento(intensidad, Vector3D(0, 0, 0), radio)//dummy
	{
	};
	virtual ~Torbellino() {};
	void updateFuerzas(Particula* p, double t = 0.0) override;
private:
	Vector3D centro;
	float K;//cte del torbellino
	float radio;
};

