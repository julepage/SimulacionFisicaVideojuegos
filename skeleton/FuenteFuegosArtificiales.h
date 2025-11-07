#pragma once
#include "FuenteParticulas.h"
#include <random>

class FuenteFuegosArtificiales : public FuenteParticulas {
public:
    FuenteFuegosArtificiales(Particula* p, Vector3D pos, float velIni, float tasa, float dist)
        : FuenteParticulas(p, pos, Vector3D(0, 1, 0), velIni, tasa, dist),//lo disparo hacia arriba
        generador(std::random_device{}()), distColor(0.0f, 1.0f) {
    }

    virtual ~FuenteFuegosArtificiales() override = default;

    void emitir(float t) override;
    void actualizar(float t) override;

private:
    std::mt19937 generador;
    std::uniform_real_distribution<float> distColor;
};
