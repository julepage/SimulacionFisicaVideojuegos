#include "Explosion.h"

void Explosion::updateFuerzas(Particula* p, double dt)
{
	if (!activa) return;
	if (!p->getHayFuerza()) return;
	if (!p->permiteFuerza("explosion")) return;

	Vector3D distancia = p->getPos() - centro;//direccion que va a llevar la particula mas la velocidad anadida
	float d = distancia.modulo();

	//radio actual (si hay velocidad de expansión)
	float Ractual = r + vel * t;

	if (d < Ractual && d > 0.0f) {
		//fuerza que decae con distancia y tiempo
		float magnitud = K / (d * d) * physx::PxExp(-t / cteTiempo);
		Vector3D fuerza = distancia * magnitud;
		if (p->getHayFuerza())
			p->addFuerza(fuerza);
	}

}

void Explosion::update(double dt)
{
	if (activa) t += dt;//aumenta tiempo q pasa
	if (activa && t > 2.0f) toggle();
}
