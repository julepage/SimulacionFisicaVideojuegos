#include "Muelle.h"

void Muelle::updateFuerzas(Particula* p, double t)
{
	if (!activa) return;
	if (!p->permiteFuerza("muelle")) return;

	Vector3D d = p->getPos() - pos;
	float dist = d.modulo();
	if (dist == 0) return;

	Vector3D dir = d / dist;

	float deformacion = dist - longitud;

	//fuerza del muelle
	Vector3D fuerza = dir * (-k * deformacion);

	p->addFuerza(fuerza);

}

void Muelle::toggle()
{
	GeneradorFuerzas::toggle();
	if (ancla != nullptr) {
		DeregisterRenderItem(ancla);
		ancla = nullptr;
	}
	else {
		ancla = new RenderItem(CreateShape(PxBoxGeometry(1.0f, 1.0f, 1.0f)),
			new PxTransform(PxVec3(pos.getX(), pos.getY(), pos.getZ())), Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	}
}
