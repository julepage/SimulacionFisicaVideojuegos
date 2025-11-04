#include "Particula.h"
#include <iostream>
using namespace physx;

physx::PxSphereGeometry Particula::defaultSphere(1.0f);

Particula::Particula(Vector3D pos, Vector3D vel, Vector3D acc, Vector4 color, physx::PxGeometry* forma, float d, float mass, float g, float vidas): pose(pos.getX(), pos.getY(), pos.getZ()), velocidad(vel),
ac(acc), damping(d), masa(mass), gravedad(g), vida(vidas), posIni(pos), fuerzaAcumulada(0, 0, 0), color(color), forma(forma)
{
	if (forma == nullptr)
		this->forma = &defaultSphere;
	else
		this->forma = forma;
	renderItem = new RenderItem(CreateShape(*this->forma), &pose,color);//le tengo que pasar la direccion de donde apunta pose

}


void Particula::integrate(double t)
{
	Vector3D acTotal = (fuerzaAcumulada / masa) + ac;
	
	velocidad = velocidad * pow(damping, t) + (acTotal * t);

	pose.p.x += velocidad.getX() * t;
	pose.p.y += velocidad.getY() * t;
	pose.p.z += velocidad.getZ() * t;

	// resetear fuerzas acumuladas para el siguiente paso
	fuerzaAcumulada = Vector3D(0, 0, 0);

}
