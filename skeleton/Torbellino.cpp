#include "Torbellino.h"
#include <cmath>

void Torbellino::updateFuerzas(Particula* p, double t)
{
    Vector3D r = p->getPos() - centro;
    float d = r.modulo();
    if (d > 0.0f && d < radio) {
        Vector3D tangencial(-r.getY(), r.getX(), 0.0f);
        tangencial = tangencial.normalize();

        // fuerza proporcional a la intensidad y decae ligeramente con la distancia
        float magnitud = K * (1.0f - d / radio); // máximo en el centro, 0 en el borde
        Vector3D f = tangencial * magnitud;

        p->addFuerza(f);
    }
}
