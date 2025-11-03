#include "Explosion.h"

void Explosion::updateFuerzas(Particula* p, double dt)
{
	if (activa) {
        Vector3D distancia = p->getPos() - centro;//direccion que va a llevar la particula mas la velocidad anadida
        float d = distancia.modulo();

        //radio actual (si hay velocidad de expansión)
        float Ractual = r + vel * t;

        if (d < Ractual && d > 0.0f) {
            //fuerza radial que decae con distancia y tiempo
            float magnitud = K / (d * d) * physx::PxExp(-t / cteTiempo);
            Vector3D fuerza = distancia * magnitud; 
            p->addFuerza(fuerza);
        }
	}
	else return;
}

void Explosion::update(double dt)
{
    if (activa) t += dt;//aumenta tiempo q pasa
}
