#pragma once
#include "Particula.h"


class Proyectil : public Particula
{
public:

	Proyectil(Vector3D pos, Vector3D velDir, float vR, float vS, float masaR, Vector4 color = Vector4(1.0f, 1.0f, 0.0f, 1.0f), physx::PxGeometry* forma = nullptr, float d = 1.0, float gR = 9.8);

	virtual ~Proyectil() {};

	// para el futuro el tiro de la bola de golf
	void aplicaVel(const Vector3D& v) noexcept { this->velocidad = v; disparado = true; };
	void setInteractuable(bool s) noexcept { interactuable = s; };
	bool esInteractuable() const noexcept { return interactuable && !disparado; };
	bool isDisparado() const noexcept { return disparado; };

protected:
	//no se borran, no me interesa porque en mi juego habrá uno que si se pierde, se reposiciona
	float calcularMasaSimulada(float masaR, float velR, float velS);
	float gravedadSimulada(float gR, float velR, float velS);

private:
	bool interactuable = true;
	bool disparado = false;
	Vector3D posIni;
};

