#pragma once
#include "GeneradorFuerzas.h"
#include "Particula.h"

//funciona con tiempo
class Explosion : public GeneradorFuerzas
{
public:
	Explosion(float intensidad, float radioIni, Vector3D centro, float tau, float velExpansion = 0.0f) :
		K(intensidad), r(radioIni), centro(centro), cteTiempo(tau), vel(velExpansion) {
	};
	void updateFuerzas(Particula* p, double t) override;
	void update(double t);
	void explosion() { activa = !activa; t = 0.0f; };
	virtual ~Explosion() {};
private:
	float K;//intensidad de la explosión
	float r;//radio ini
	float cteTiempo;//tau: cte de tiempo de decaimiento
	float vel;//vel de expansión 
	Vector3D centro;//centro de la explosión
	float t = 0.0f;//tiempo que pasa desde que se activó
	bool activa = false;
};

