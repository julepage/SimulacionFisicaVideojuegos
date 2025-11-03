#include "Particula.h"
#include <iostream>
using namespace physx;

Particula::Particula(Vector3D pos, Vector3D vel, Vector3D acc, float d, float mass, float g, float vidas) : pose(pos.getX(), pos.getY(), pos.getZ()), velocidad(vel),
ac(acc), damping(d), masa(mass), gravedad(g), vida(vidas), posIni(pos), fuerzaAcumulada(0, 0, 0)
{
	renderItem = new RenderItem(CreateShape(PxSphereGeometry(1.0f)), &pose, Vector4(1.0f, 1.0f, 0.0f, 1.0f));//le tengo que pasar la direccion de donde apunta pose

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
