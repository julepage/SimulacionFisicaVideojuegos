#pragma once
#include "Viento.h"

class Torbellino : public Viento
{
public:
	Torbellino(Vector3D c, float intensidad1, float intensidad2, float r) : centro(c), K1(intensidad1), K2(intensidad2), radio(r), Viento(intensidad1, Vector3D(0, 0, 0), radio)//dummy
	{
	};
	virtual ~Torbellino() {};
	void updateFuerzas(Particula* p, double t = 0.0) override;
private:
	Vector3D centro;
	float K1;//cte del torbellino
	float K2;//cte del torbellino
	float radio;
};

