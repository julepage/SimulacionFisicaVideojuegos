#pragma once
#include "FuenteParticulas.h"
#include <random>

class FuenteFuego : public FuenteParticulas {
public:
    FuenteFuego(Vector3D pos, float velIni, float tasa, float longitud, float dist)
        : FuenteParticulas(pos, Vector3D(0, 1, 0), velIni, tasa, dist),
        generador(std::random_device{}()),
        distColor(0.8f, 1.0f),
        distLinea(0.0f, longitud / 6.0f)
    {};

    virtual ~FuenteFuego() override = default;

    void emitir(float t) override;
    void actualizar(float t) override;

private:
    std::mt19937 generador;
    //DISTRIBUCIONES NORMALES Y UNIFORME
    std::uniform_real_distribution<float> distColor;//para los colores del fuego
    std::normal_distribution<float> distLinea;//para donde salen
};
