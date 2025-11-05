#include "Proyectil.h"
#include "Vector3D.h"


Proyectil::Proyectil(Vector3D pos, Vector3D velDir, float vR, float vS, float masaR, Vector4 color, physx::PxGeometry* forma, float d, float gR):
	Particula(pos, velDir.normalize()* vS, Vector3D(0, 0, 0), color, forma, d, calcularMasaSimulada(masaR, vR, vS), gravedadSimulada(gR, vR, vS))
{
	setAc(Vector3D(0.0f, -gravedad, 0.0f));
};

float Proyectil::calcularMasaSimulada(float masaR, float velR, float velS)
{
    return (masaR * velR * velR) / (velS * velS);
}

float Proyectil::gravedadSimulada(float gR, float velR, float velS)
{
    return (gR * velS * velS) / (velR * velR);
}

