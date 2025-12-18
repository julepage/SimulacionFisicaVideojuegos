#include "Muelle.h"
#include <iostream>
using namespace physx;
using namespace std;

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

void Muelle::update(float deltaTime)
{
	// Constantes del muelle
	float k = 500.0f;      // Constante del muelle (ajusta según tu escala)
	float longitud = 2.0f; // Longitud natural del muelle (metros)

	// Posición del anclaje (debe ser un PxVec3 válido)
	PxVec3 posAncla = PxVec3(450, 80, 40); // ejemplo, pon tu valor real

	// Posición actual de la puerta
	PxVec3 posPuerta = puerta->getCuerpoDinamico()->getGlobalPose().p;

	// Vector desde el anclaje hasta la puerta
	PxVec3 d = posPuerta - posAncla;
	float dist = d.magnitude();

	// Evitamos división por cero
	if (dist < 0.0001f) return;

	// Dirección normalizada
	PxVec3 dir = d / dist;

	// Deformación del muelle
	float deformacion = dist - longitud;

	// Fuerza del muelle (Hacia el anclaje)
	PxVec3 fuerza = -k * deformacion * dir;

	// Aplicamos la fuerza al cuerpo de la puerta
	puerta->getCuerpoDinamico()->addForce(fuerza);
	
}
