#include "Pelota.h"
#include "globales.h"
#include  <iostream>

Pelota::Pelota(PxPhysics* physics, PxScene* scene, PxMaterial* mat,
	Vector3D posInicial, float radio, Vector4 colorBola)
	: posI(posInicial), color(colorBola), scene(scene)
{
	cuerpo = physics->createRigidDynamic(PxTransform(posInicial.getX(), posInicial.getY(), posInicial.getZ()));
	PxShape* shape = physics->createShape(PxSphereGeometry(radio), *mat, true);
	cuerpo->attachShape(*shape);
	PxRigidBodyExt::updateMassAndInertia(*cuerpo, 1.0f);
	scene->addActor(*cuerpo);

	cuerpo->setLinearDamping(0.5f);
	render = new RenderItem(shape, cuerpo, color);
	cuerpo->putToSleep();
}

Pelota::~Pelota()
{
	if (render) DeregisterRenderItem(render);
	if (cuerpo) cuerpo->release();
}

Vector3D Pelota::getPos() const
{
	auto p = cuerpo->getGlobalPose().p;
	return { p.x, p.y, p.z };
}
void Pelota::comenzarArrastre()
{
	arrastrando = true;
	puntoInicio = getPos();    // origen = bola
	puntoActual = puntoInicio; // inicializar
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

	// Vector de fuerza desde la bola hacia el mouse proyectado
	Vector3D dir = puntoActual - puntoInicio;
	float coef = 20.0f; // ajustar fuerza
	dir = dir * coef;

	PxVec3 fuerza(dir.getX(), dir.getY(), dir.getZ());
	cuerpo->wakeUp();
	movida = true;
	cuerpo->setLinearDamping(0.5);
	cuerpo->addForce(-fuerza, PxForceMode::eIMPULSE);
}

Vector3D Pelota::screenToWorld(int mouseX, int mouseY)
{
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);

	// Normalizar coordenadas del mouse a [-1,1]
	float nx = (2.0f * mouseX) / width - 1.0f;
	float ny = 1.0f - (2.0f * mouseY) / height;

	// Cámara
	Vector3D camPos(GetCamera()->getEye().x, GetCamera()->getEye().y, GetCamera()->getEye().z);
	Vector3D camDir(GetCamera()->getDir().x, GetCamera()->getDir().y, GetCamera()->getDir().z);
	camDir = camDir.normalize();

	// Ejes de cámara
	Vector3D up(0, 1, 0);
	Vector3D right = camDir.cross(up).normalize();
	Vector3D cameraUp = right.cross(camDir).normalize();

	// Bola
	Vector3D bola = getPos();

	// Vector desde la bola hacia la cámara
	Vector3D dirBolaCam = camPos - bola;
	float distance = dirBolaCam.modulo();
	dirBolaCam = dirBolaCam.normalize();

	// Factor de desplazamiento según el ratón
	float factor = distance * 0.5f; // ajustar sensibilidad

	// Desplazamiento lateral y vertical relativo al plano de la cámara
	Vector3D offset = right * nx * factor + cameraUp * ny * factor;

	// Posición final en el mundo: desde la bola hacia la cámara + offset lateral/vertical
	Vector3D worldPos = bola + dirBolaCam * distance + offset;

	return worldPos;
}


void Pelota::reset()
{
	if (!cuerpo) return;
	/*cuerpo->setLinearVelocity(PxVec3(0, 0, 0));
	cuerpo->setAngularVelocity(PxVec3(0, 0, 0));*/
	cuerpo->putToSleep();
	cuerpo->setGlobalPose(PxTransform(posI.getX(), posI.getY(), posI.getZ()));
	cuerpo->wakeUp();
}

bool Pelota::raycast(const PxVec3& origen, const PxVec3& direccion, PxRaycastBuffer& hit)
{
	// Raycast solo contra la esfera de la pelota
	PxVec3 pos = cuerpo->getGlobalPose().p;
	float radio = 1.0f; // o el radio que tenga tu pelota

	// Aquí puedes hacer un ray-sphere test sencillo
	PxVec3 oc = origen - pos;
	float b = oc.dot(direccion);
	float c = oc.dot(oc) - radio * radio;
	float discriminante = b * b - c;
	return (discriminante > 0);
}

bool Pelota::estaParada(float umbral) const
{
	if (!cuerpo) return true;

	PxVec3 vel = cuerpo->getLinearVelocity();
	PxVec3 angVel = cuerpo->getAngularVelocity();

	// Comprobamos que tanto velocidad lineal como angular sean muy pequeñas
	return vel.magnitude() < umbral || angVel.magnitude() < umbral;

}

void Pelota::update(float deltaTime)
{
	if (!cuerpo) return;
	cuerpo->setLinearDamping(cuerpo->getLinearDamping() + 0.0001);
	//if (cuerpo->getLinearVelocity().magnitude() > 0) 

	std::cout << cuerpo->getLinearVelocity().x << std::endl;
}

