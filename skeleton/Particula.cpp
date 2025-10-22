#include "Particula.h"
using namespace physx;

Particula::Particula(Vector3D pos, Vector3D vel, Vector3D acc, float d, float mass, float g, float vidas): pose(pos.getX(), pos.getY(), pos.getZ()), velocidad(vel),
ac(acc), damping(d), masa(mass), gravedad(g), vida(vidas), posIni(pos)
{
	renderItem = new RenderItem(CreateShape(PxSphereGeometry(1.0f)), &pose, Vector4(1.0f, 1.0f, 0.0f, 1.0f));//le tengo que pasar la direccion de donde apunta pose

}


void Particula::integrate(double t)
{

	velocidad = velocidad * pow(damping, t) + (ac * t);

	pose.p.x += velocidad.getX() * t;
	pose.p.y += velocidad.getY() * t;
	pose.p.z += velocidad.getZ() * t;

}
