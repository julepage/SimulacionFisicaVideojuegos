#pragma once
#include "Particula.h"
#include <random> 

class FuenteParticulas
{
public:
	FuenteParticulas(Particula* p, Vector3D posi, Vector3D dire, float velo, float tasa, float dist) : pos(posi), vel(velo),
		tasaEmision(tasa), generador(std::random_device{}()), distUniforme(-dist, dist), modelo(p)
	{
		dir = dire.normalize();
	};

	virtual ~FuenteParticulas() {
		for (auto p : particulas) delete p;
		particulas.clear();
		delete modelo;
	};

	virtual void emitir(float t) = 0;//crear particulas
	virtual void actualizar(float t);//mueve part

	//getter
	std::vector<Particula*>& getParticulas() { return particulas; }
	const Vector3D& getPos() const { return pos; }

protected:
	Vector3D pos;//de donde sale
	Vector3D dir;//hacia donde sale
	float vel;//velocidad con la que sale
	float tasaEmision;//par emitidas por frame
	std::vector<Particula*> particulas;
	Particula* modelo;

	//distrib uniforme desde -x hasta x 
	std::mt19937 generador;//usa semilla aleatoria
	std::uniform_real_distribution<double> distUniforme;//cojo los aleat del generador y los pongo en rango deseado

};

