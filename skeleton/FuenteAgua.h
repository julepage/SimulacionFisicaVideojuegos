#pragma once
#include "FuenteParticulas.h"

class FuenteAgua: public FuenteParticulas
{
public:
	FuenteAgua(Vector3D pos, Vector3D dir, float vel, float tasa)
		: FuenteParticulas(pos, dir, vel, tasa)
	{
	};
	virtual void emitir(float t) override;

};

