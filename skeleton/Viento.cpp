#include "Viento.h"
#include <iostream>

void Viento::updateFuerzas(Particula* p, double t)
{
	//de momento k2 = 0
	fuerzaViento = (vel - p->getVel()) * k1;
	p->addFuerza(fuerzaViento);
}
