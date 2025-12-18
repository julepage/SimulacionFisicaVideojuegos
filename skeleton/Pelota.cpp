#include "Pelota.h"
#include "globales.h"
#include  <iostream>

Pelota::Pelota(PxPhysics* physics, PxScene* scene, PxMaterial* mat,
	Vector3D posInicial, float radio, Vector4 colorBola)
	: Solido(physics, scene, mat, posInicial, radio, colorBola, Solido::Tipo::Dinamico, Solido::TipoEspecial::Pelota),
	posI(posInicial), color(colorBola), scene(scene)
{
	cuerpoD->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_X, false);
	cuerpoD->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, false);
	cuerpoD->setLinearDamping(0.01f);
	
	cuerpoD->putToSleep();
}

Vector3D Pelota::getPos() const
{
	auto p = cuerpoD->getGlobalPose().p;
	return { p.x, p.y, p.z };
}
void Pelota::comenzarArrastre()
{
	arrastrando = true;
	puntoInicio = getPos();    
	puntoActual = puntoInicio; 
}

void Pelota::actualizarArrastre(int mouseX, int mouseY)
{
	if (!arrastrando) return;
	puntoActual = screenToWorld(mouseX, mouseY);
}

void Pelota::soltarArrastre()
{
	if (!arrastrando) return;
	arrastrando = false;

	//vector de fuerza desde la bola hacia el mouse proyectado
	Vector3D dir = puntoActual - puntoInicio;
	float coef = 60.0f;
	dir = dir * coef;

	PxVec3 fuerza(dir.getX(), dir.getY(), dir.getZ());
	cuerpoD->wakeUp();
	movida = true;
	cuerpoD->setLinearDamping(0.5);
	cuerpoD->addForce(-fuerza, PxForceMode::eIMPULSE);
}

Vector3D Pelota::screenToWorld(int mouseX, int mouseY)
{
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);

	float nx = (2.0f * mouseX) / width - 1.0f;
	float ny = 1.0f - (2.0f * mouseY) / height;

	//cam
	Vector3D camPos(GetCamera()->getEye().x, GetCamera()->getEye().y, GetCamera()->getEye().z);
	Vector3D camDir(GetCamera()->getDir().x, GetCamera()->getDir().y, GetCamera()->getDir().z);
	camDir = camDir.normalize();

	Vector3D up(0, 1, 0);
	Vector3D right = camDir.cross(up).normalize();
	Vector3D cameraUp = right.cross(camDir).normalize();

	//bola
	Vector3D bola = getPos();

	//bola hacia la cam
	Vector3D dirBolaCam = camPos - bola;
	float distance = dirBolaCam.modulo();
	dirBolaCam = dirBolaCam.normalize();

	float factor = distance * 0.5f;
	Vector3D offset = right * nx * factor + cameraUp * ny * factor;
	Vector3D worldPos = bola + dirBolaCam * distance + offset;

	return worldPos;
}


void Pelota::reset()
{
	if (!cuerpoD) return;
	

	cuerpoD->putToSleep();
	cuerpoD->setGlobalPose(PxTransform(posI.getX(), posI.getY(), posI.getZ()));
	cuerpoD->wakeUp();
	
}

bool Pelota::raycast(const PxVec3& origen, const PxVec3& direccion, PxRaycastBuffer& hit)
{

	PxVec3 pos = cuerpoD->getGlobalPose().p;
	float radio = 1.0f; 

	
	PxVec3 oc = origen - pos;
	float b = oc.dot(direccion);
	float c = oc.dot(oc) - radio * radio;
	float discriminante = b * b - c;
	return (discriminante > 0);
}

bool Pelota::estaParada(float umbral) const
{
	if (!cuerpoD) return true;

	PxVec3 vel = cuerpoD->getLinearVelocity();

	// comprobamos que tanto velocidad lineal como angular sean muy pequeñas
	return vel.magnitude() < umbral;

}



