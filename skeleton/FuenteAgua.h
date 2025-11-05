#pragma once
#include "FuenteParticulas.h"

class FuenteAgua: public FuenteParticulas
{
public:
	FuenteAgua(Vector3D pos, Vector3D dir, float vel, float tasa, float dist)
		: FuenteParticulas(pos, dir, vel, tasa, dist)
	{
	};
	virtual void emitir(float t) override;

};

