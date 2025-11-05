#pragma once
#include "Particula.h"


class Proyectil : public Particula
{
public:

	Proyectil(Vector3D pos, Vector3D velDir, float vR, float vS, float masaR, Vector4 color = Vector4(1.0f, 1.0f, 0.0f, 1.0f), physx::PxGeometry* forma = nullptr, float d = 1.0, float gR = 9.8);

	virtual ~Proyectil() {};

	// para el futuro el tiro de la bola de golf
	void applyExternalVelocity(const Vector3D& v) noexcept { this->velocidad = v; disparado = true; }
	void setSelectable(bool s) noexcept { selectable = s; }
	bool isSelectable() const noexcept { return selectable && !disparado; }
	bool isDisparado() const noexcept { return disparado; }


protected:

	float calcularMasaSimulada(float masaR, float velR, float velS);
	float gravedadSimulada(float gR, float velR, float velS);

private:
	bool selectable = true;
	bool disparado = false;
};

