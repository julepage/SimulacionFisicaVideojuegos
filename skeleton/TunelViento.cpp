#include "TunelViento.h"

void TunelViento::updateFuerzas(Particula* p, double t)
{
    if (!activa) return;
    Vector3D pos = p->getPos();
    Vector3D dif = pos - centro;
    float distancia = dif.modulo();

    //si mi particuaa esta en el radio
    if (distancia <= radio) {
        // La fuerza crece conforme se acerca al centro (opcional)
        float factor = 1.0f - (distancia / radio);  // 1 en el centro, 0 en el borde
        Vector3D fuerza = Vector3D(0, 1, 0) * intensidad * factor;
        p->addFuerza(fuerza);
    }
}
