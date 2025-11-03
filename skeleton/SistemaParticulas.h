#pragma once
#include "FuenteParticulas.h"

class SistemaParticulas
{
public:
	SistemaParticulas(FuenteParticulas& f) : fuente(f) {}

	void mata(double t) {

		auto& particulas = fuente.getParticulas(); //referencia a vector particulas

		for (auto it = particulas.begin(); it != particulas.end(); ) {//iterador
			Particula* p = *it;//cojo a la particula

            Vector3D diferencia = p->getPos() - p->getPosIni();
            float distancia = diferencia.modulo();//ya es como valor absoluto, es el modulo de la distancia
            
            if (p->getVidas() <= 0.0f || distancia > RADIO_MAX) {
                delete p;
                it = particulas.erase(it);//actualizo puntero it
            }
            else {
                ++it;
            }
		}
	}

	virtual ~SistemaParticulas() {}
private:
	FuenteParticulas& fuente;
    //distancia maxima que se pueden alejar sin ser eliminadas
    const float RADIO_MAX = 200.0f;
};

