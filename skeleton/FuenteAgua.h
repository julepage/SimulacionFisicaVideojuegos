#pragma once
#include "FuenteParticulas.h"

class FuenteAgua: public FuenteParticulas
{
public:
	FuenteAgua(Particula* p, Vector3D pos, Vector3D dir, float vel, float tasa, float dist)
		: FuenteParticulas(p, pos, dir, vel, tasa, dist)
	{
	};
	virtual void emitir(float t) override;

};

