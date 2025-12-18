#include "Viento.h"
#include <iostream>

void Viento::updateFuerzas(Particula* p, double t)
{
	if (!activa) return;
	if (!p->getHayFuerza()) return;
	if (!p->permiteFuerza("viento")) return;
	//de momento k2 = 0
	fuerzaViento = (vel - p->getVel()) * k1;
	p->addFuerza(fuerzaViento);
}


void Viento::updateFuerzas(Solido* s, double t)
{
    if (!activa) return;
    if (!s || s->getTipo() != Solido::Tipo::Dinamico) return;
    PxRigidDynamic* cuerpo = s->getCuerpoDinamico();
    if (!cuerpo) return;

    Vector3D posSolido(cuerpo->getGlobalPose().p.x,
        cuerpo->getGlobalPose().p.y,
        cuerpo->getGlobalPose().p.z);

    // Comprobar si está dentro del radio de efecto
    if (radio > 0) {
        Vector3D offset = posSolido - pos;
        if (offset.modulo() > radio) return; // fuera del área de efecto
    }

    // Fuerza aplicada
    Vector3D dir =  vel - Vector3D(s->getCuerpoDinamico()->getLinearVelocity().x, s->getCuerpoDinamico()->getLinearVelocity().y, s->getCuerpoDinamico()->getLinearVelocity().z);
    Vector3D fuerza = dir * k1;
    cuerpo->addForce(PxVec3(fuerza.getX(), fuerza.getY(), fuerza.getZ()), PxForceMode::eFORCE);
}