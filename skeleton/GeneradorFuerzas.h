#pragma once
#include "Particula.h"

//clase abstarcta para poder generar fuerzas a partir de esta

class GeneradorFuerzas
{
public:
	virtual ~GeneradorFuerzas() = default;
	virtual void updateFuerzas(Particula* p, double t = 0.0) = 0;
	virtual void toggle() { activa = !activa; }
	virtual void update(double dt) {} // por defecto no hace nada pero es por si necesito actualizar por frmae y no por particulas como en la explosion
protected:
	bool activa = false;//apagada por defecto
};

