#include "TunelViento.h"

void TunelViento::updateFuerzas(Particula* p, double t)
{
	if (!activa) return;
	if (!p->getHayFuerza()) return;
	if (!p->permiteFuerza("tunelViento")) return;
	Vector3D pos = p->getPos();
	Vector3D dif = pos - centro;
	float distancia = dif.modulo();

	//si mi particuaa esta en el radio
	if (distancia <= radio) {
		// La fuerza crece conforme se acerca al centro
		float factor = 1.0f - (distancia / radio);//1 en el centro ,0 en el borde
		Vector3D fuerza = Vector3D(0, 1, 0) * intensidad * factor;
		if (p->getHayFuerza())
			p->addFuerza(fuerza);
	}
}

void TunelViento::updateFuerzas(Pelota* pelota, double t)
{
	if (!activa) return;
	if (!pelota) return;

	Vector3D pos = pelota->getPos();
	Vector3D dif = pos - centro;
	float distancia = dif.modulo();

	if (distancia <= radio) {
		// La fuerza aumenta al acercarse al centro
		float factor = 1.0f - (distancia / radio); // 1 en el centro, 0 en el borde
		Vector3D fuerza = Vector3D(0, 1, 0) * intensidad * factor;

		pelota->aplicarFuerza(fuerza);
	}
}