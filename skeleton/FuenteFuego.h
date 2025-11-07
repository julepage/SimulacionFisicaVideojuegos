#pragma once
#include "FuenteParticulas.h"
#include <random>

class FuenteFuego : public FuenteParticulas {
public:
    FuenteFuego(Particula* p, Vector3D pos, float velIni, float tasa, float longitud, float dist)
        : FuenteParticulas(p, pos, Vector3D(0, 1, 0), velIni, tasa, dist),
        generador(std::random_device{}()),
        distLinea(0.0f, longitud / 6.0f)
    {};

    virtual ~FuenteFuego() override = default;

    void emitir(float t) override;
    void actualizar(float t) override;

private:
    std::mt19937 generador;
    //DISTRIBUCIONES NORMALES
    std::normal_distribution<float> distLinea;//para donde salen
};
