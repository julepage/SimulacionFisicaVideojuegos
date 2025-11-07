#pragma once
#include "Viento.h"

class TunelViento : public Viento {
public:
    TunelViento(Vector3D centro, float intensidad, float radio)
        : Viento(intensidad, Vector3D(0, 1, 0), 0.0f), // viento vertical hacia arriba
        centro(centro), radio(radio), intensidad(intensidad) {
    }
    void updateFuerzas(Particula* p, double t = 0.0) override;
private:
    Vector3D centro;
    float radio;
    float intensidad;
};


