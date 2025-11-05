#include "Torbellino.h"
#include <cmath>

void Torbellino::updateFuerzas(Particula* p, double t)
{
	//SI LA FUENTE TIENE VIENTO ACTIVO, LE VA A AFECTAR ESTE VIENTO TAMBIEN
	if (!activa) return;
	if (!p->getHayFuerza()) return;
	if (!p->permiteFuerza("torbellino")) return;
	
	//Este es torbellino hacia arriba
	if (!p || p->getMasa() <= 0.0f) return;

	Vector3D pos = p->getPos();
	Vector3D delta = pos - centro;
	float d = delta.modulo();

	// Solo si está dentro del radio del torbellino
	if (d > radio || d < 1e-3f) return;

	// Vector tangencial (en plano XZ)
	Vector3D tangencial(-delta.getZ(), 0, delta.getX());
	tangencial.normalize();

	// Fuerza rotacional (proporcional a K1)
	Vector3D fuerza = tangencial * K1 * (1.0f - d / radio);

	// Chapuza: metemos un empujón vertical (cuanto más cerca, más sube)
	fuerza.setY(K2 * (1.0f - d / radio));

	// También tiramos un poco hacia el centro para que no se escapen
	Vector3D haciaCentro = delta * (-0.2f * (1.0f - d / radio));
	fuerza = fuerza + haciaCentro;

	p->addFuerza(fuerza);

}
