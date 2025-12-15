#pragma once
#include "FuenteParticulas.h"
#include "GeneradorFuerzas.h"

class SistemaParticulas
{
public:
	SistemaParticulas() = default;

	virtual ~SistemaParticulas() {
		for (auto f : fuentes)
			delete f;
		fuentes.clear();
		for (auto f : fuerzas)
			delete f;
		fuerzas.clear();
		for (auto p : particulas)
			delete p;
		particulas.clear();
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
		for (auto f : fuerzas)
			f->update(t); //para las explosiones que van por frame

		for (auto f : fuentes)
		{
			f->emitir(t);
			f->actualizar(t);
			for (auto p : f->getParticulas()) {

				for (auto f : fuerzas)
				{
					f->updateFuerzas(p, t);
					
				}
			}
		}

		//particulas
		for (auto p : particulas) {
			p->integrate(t);//no actualiza sus vidas, no me interesa que se borren de momento
			for (auto f : fuerzas)
				f->updateFuerzas(p, t);
		}

	}

	void addFuente(FuenteParticulas* fuente)
	{
		fuentes.push_back(fuente);
	}

	void addParticula(Particula* p) {
		particulas.push_back(p);
	}

	void eliminarFuente(FuenteParticulas* fuente)
	{
		auto it = std::find(fuentes.begin(), fuentes.end(), fuente);
		if (it != fuentes.end()) {
			delete* it;
			fuentes.erase(it);
		}
	}

	void eliminaParticula(Particula* p) {
		auto it = std::find(particulas.begin(), particulas.end(), p);
		if (it != particulas.end()) {
			delete* it;
			particulas.erase(it);
		}
	}

	void addFuerza(GeneradorFuerzas* g) {
		fuerzas.push_back(g);
	}

	//getter
	size_t getNumFuentes() const { return fuentes.size(); }
	const std::vector<FuenteParticulas*>& getFuentes() const { return fuentes; }


private:
	std::vector<FuenteParticulas*> fuentes;
	std::vector<Particula*> particulas;
	std::vector<GeneradorFuerzas*> fuerzas;
	//distancia maxima que se pueden alejar sin ser eliminadas
	const float radioMax = 200.0f;
};

