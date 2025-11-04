#pragma once
#include "FuenteParticulas.h"
#include <random>

class FuenteFuego : public FuenteParticulas {
public:
    FuenteFuego(Vector3D pos, float velIni, float tasa, float longitud)
        : FuenteParticulas(pos, Vector3D(0, 1, 0), velIni, tasa),
        generador(std::random_device{}()), distColor(0.8f, 1.0f), distLinea(-longitud / 2, longitud / 2)
    {
    }

    virtual ~FuenteFuego() override = default;

    void emitir(float t) override {
        int numParticulas = static_cast<int>(tasaEmision * t);
        if (numParticulas <= 0) numParticulas = 1;

        for (int i = 0; i < numParticulas; i++) {
            //se mueve por la linea
            Vector3D posLinea = pos;
            posLinea.setX(posLinea.getX() + distLinea(generador));

            //vel
            Vector3D velParticula = dir * vel;
            velParticula.setX(velParticula.getX() + distLinea(generador) * 0.1f);
            velParticula.setZ(velParticula.getZ() + distLinea(generador) * 0.1f);

            //fuego
            Vector4 color(distColor(generador), distColor(generador) * 0.5f, 0.0f, 1.0f);

            Particula* p = new Particula(posLinea, velParticula, Vector3D(0, -9.8f, 0),
                color, nullptr, 0.95f, 0.5f, 9.8f, 2.0f);
            particulas.push_back(p);
        }
    }

    void actualizar(float t) override {
        std::vector<Particula*> nuevas;
        for (auto p : particulas) {
            p->integrate(t);
            // Reducir vida de la partícula
            p->setVidas(p->getVidas() - t);
            if (p->getVidas() > 0.0f) {
                nuevas.push_back(p);
            }
            else {
                delete p;
            }
        }
        particulas = nuevas;
    }

private:
    std::mt19937 generador;
    std::uniform_real_distribution<float> distColor;
    std::uniform_real_distribution<float> distLinea;
};
