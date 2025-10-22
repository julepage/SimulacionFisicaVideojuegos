#pragma once
#include "Particula.h"
#include <random> 

class FuenteParticulas
{
public:
	FuenteParticulas(Vector3D posi, Vector3D dire, float velo, float tasa) : pos(posi), vel(velo),
		tasaEmision(tasa), generador(std::random_device{}()), distUniforme(-2, 2)
	{
		dir = dire.normalize();
	};

	virtual ~FuenteParticulas() {
		for (auto p : particulas) delete p;
		particulas.clear();
	};

	void emitir(float t);//crear particulas
	void actualizar(float t);//mueve, elimina... particulas

	//getter
	std::vector<Particula*>& getParticulas() { return particulas; }


protected:
	Vector3D pos;//de donde sale
	Vector3D dir;//hacia donde sale
	float vel;//velocidad con la que sale
	float tasaEmision;//cuantas part se emiten por frame
	std::vector<Particula*> particulas;

	//distrib uniforme desde -x hasta x de numeros aleatorios
	std::mt19937 generador;//usa semilla aleatoria
	std::uniform_real_distribution<double> distUniforme;//cojo los aleat del generador y los pongo en rango deseado

};

