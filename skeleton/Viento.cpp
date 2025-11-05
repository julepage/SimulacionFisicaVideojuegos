#include "Viento.h"
#include <iostream>

void Viento::updateFuerzas(Particula* p, double t)
{
	if (!activa) return;
	if (!p->getHayFuerza()) return;
	if (!p->permiteFuerza("viento")) return;
	//de momento k2 = 0
	fuerzaViento = (vel - p->getVel()) * k1;
	p->addFuerza(fuerzaViento);
}
