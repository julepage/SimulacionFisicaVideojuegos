#include "Particula.h"
using namespace physx;

Particula::Particula(Vector3D pos, Vector3D vel, float acc, float d) : pose(pos.getX(), pos.getY(), pos.getZ()), velocidad(vel), ac(acc), damping(d)
{
	renderItem = new RenderItem(CreateShape(PxSphereGeometry(1.0f)), &pose, Vector4(1.0f, 1.0f, 0.0f, 1.0f));//le tengo que pasar la direccion de donde apunta pose

}

void Particula::integrate(double t)
{
	velocidad = velocidad + ac * t;
	velocidad = velocidad.numeroPorVec(pow(damping, t));

	pose.p.x += velocidad.getX() * t;
	pose.p.y += velocidad.getY() * t;
	pose.p.z += velocidad.getZ() * t;

}
