#pragma once
#include "GeneradorFuerzas.h"

class Flotacion: public GeneradorFuerzas
{
public:
	Flotacion(Vector3D pos, float s, float d, float g): pos(pos), s(s), g(g), d(d) {
	//crear plano 

	};
	~Flotacion() = default;
	void updateFuerzas(Particula* p, double t) override;
protected:
	Vector3D pos;
	float g;//gravedad
	float d;//densidad del agua
	float s;//superficie, volumen??
};


