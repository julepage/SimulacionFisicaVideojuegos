#pragma once
#include "GeneradorFuerzas.h"
#include "Solido.h"
class Viento : public GeneradorFuerzas
{
public:
	virtual ~Viento() = default;
	//el que usaba para particulas
	Viento(float velocidad, Vector3D direccion, float c) : vel(direccion.normalize()* velocidad), k1(c)
	{
	}
	Viento(Vector3D pos, float velocidad, Vector3D direccion, float c, float radio = 50.0f) : vel(direccion.normalize()* velocidad), k1(c), pos(pos), radio(radio)
	{
	}
	void updateFuerzas(Particula* p, double t = 0.0) override;
	void updateFuerzas(Solido* s, double t);
protected:
	Vector3D vel;
	float k1; // coeficiente de rozamiento con el aire
	Vector3D fuerzaViento;
	float k2 = 0;
	float radio;
	Vector3D pos;
};

