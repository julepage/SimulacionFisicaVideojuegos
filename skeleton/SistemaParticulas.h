#pragma once
#include "FuenteParticulas.h"

class SistemaParticulas
{
public:
    SistemaParticulas() = default;

	virtual ~SistemaParticulas() {
        for (auto f : fuentes)
            delete f;
        fuentes.clear();
    }

	void mata(double t) {

        for (auto* fuente : fuentes)
        {
            auto& particulas = fuente->getParticulas(); //referencia a vector particulas

            for (auto it = particulas.begin(); it != particulas.end(); ) {//iterador
                Particula* p = *it;//cojo a la particula

                Vector3D diferencia = p->getPos() - p->getPosIni();
                float distancia = diferencia.modulo();//ya es como valor absoluto, es el modulo de la distancia

                if (p->getVidas() <= 0.0f || distancia > radioMax) {
                    delete p;
                    it = particulas.erase(it);//actualizo puntero it
                }
                else {
                    ++it;
                }
            }
        }
	}

    //para actualizar todo desde el main
    void actualizar(float t)
    {
        for (auto f : fuentes)
        {
            f->emitir(t);
            f->actualizar(t);
        }
    }

    void addFuente(FuenteParticulas* fuente)
    {
        fuentes.push_back(fuente);
    }

    void eliminarFuente(FuenteParticulas* fuente)
    {
        auto it = std::find(fuentes.begin(), fuentes.end(), fuente);
        if (it != fuentes.end()) {
            delete* it;
            fuentes.erase(it);
        }
    }
    //getter
   size_t getNumFuentes() const { return fuentes.size(); }
   const std::vector<FuenteParticulas*>& getFuentes() const { return fuentes; }

    
private:
    std::vector<FuenteParticulas*> fuentes;
    //distancia maxima que se pueden alejar sin ser eliminadas
    const float radioMax = 200.0f;
};

