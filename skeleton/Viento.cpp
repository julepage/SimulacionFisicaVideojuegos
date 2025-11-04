#include "Viento.h"
#include <iostream>

void Viento::updateFuerzas(Particula* p, double t)
{
	if (!activa) return;
	//de momento k2 = 0
	fuerzaViento = (vel - p->getVel()) * k1;
	if (p->getHayFuerza())
		p->addFuerza(fuerzaViento);
}
