#include "Torbellino.h"
#include <cmath>

void Torbellino::updateFuerzas(Particula* p, double t)
{
    if (!activa) return;
    //este es un hilo que gira un poco
    //Vector3D r = p->getPos() - centro;
    //float d = r.modulo();
    //if (d > 0.0f && d < radio) {
    //    Vector3D tangencial(-r.getY(), r.getX(), 0.0f);
    //    tangencial = tangencial.normalize();

    //    // fuerza proporcional a la intensidad y decae ligeramente con la distancia
    //    float magnitud = K * (1.0f - d / radio); // máximo en el centro, 0 en el borde
    //    Vector3D f = tangencial * magnitud;

    //    p->addFuerza(f);
    //}

	//
	//auto const pos = p->getPos();

	//// Compute the whirlwind force
	//Vector3 v = p->getVel() - K1 * Vector3(-(pos.getZ() - centro.getZ()), 50 - (pos.getY() - centro.getY()), (pos.getX() - centro.getZ()));
	//float drag_coef = v.normalize();
	//Vector3 whirlwindF;
	//drag_coef = K1 * drag_coef + K2 * drag_coef * drag_coef;
	//whirlwindF = -v * drag_coef;

	//p->addFuerza(whirlwindF);

    ///Este es un disco que flota y gira raro
   // if (!p) return;//si no hay particula
   // if (p->getMasa() <= 0.0f) return;//masa inválida

   // Vector3D pos = p->getPos();
   // Vector3D dirCentro = pos - centro;
   // float distancia = dirCentro.modulo();

   // //solo afecta si esta dentro del radio
   // if (distancia > radio /*|| distancia < 1e-4f*/)
   //     return;

   // //velocidad rot respecto al centro
   // Vector3D v = p->getVel() - Vector3D(
   //     -(pos.getZ() - centro.getZ()),          
   //     50.0f - (pos.getY() - centro.getY()),   
   //     (pos.getX() - centro.getX())//si quiero q vaya mas fuerte lo multiplico aqui 
   // );

   // float mag = v.modulo();
   ///* if (mag < 1e-6f) return;*/
   // Vector3D dir = v / mag;

   // //coef arrastre
   // float drag_coef = K1 * mag + K2 * mag * mag;

   // //fuerza
   // Vector3D whirlwindF = dir * (-drag_coef);
   // p->addFuerza(whirlwindF);


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
