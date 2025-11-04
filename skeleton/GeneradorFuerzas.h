#pragma once
#include "Particula.h"

//clase abstarcta para poder generar fuerzas a partir de esta

class GeneradorFuerzas
{
public:
	virtual ~GeneradorFuerzas() = default;
	virtual void updateFuerzas(Particula* p, double t = 0.0) = 0;
	void toggle() { activa = !activa; }
protected:
	bool activa = false;//apagada por defecto
};

