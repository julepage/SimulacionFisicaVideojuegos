#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"
#include "Vector3D.h"
#include "Particula.h"
#include "Proyectil.h"
#include "FuenteParticulas.h"
#include "FuenteAgua.h"
#include "SistemaParticulas.h"
#include "Viento.h"
#include "Torbellino.h"
#include "Explosion.h"
#include "TunelViento.h"
#include "FuenteFuegosArtificiales.h"
#include "FuenteFuego.h"
#include "Muelle.h"
#include "Flotacion.h"
#include "Pelota.h"
#include "globales.h"
#include "Solido.h"
#include "SistemaSolidos.h"
#include "Mapa.h"
#include <iostream>

std::string display_text = "This is a test";


using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation* gFoundation = NULL;
PxPhysics* gPhysics = NULL;


PxMaterial* gMaterial = NULL;

PxPvd* gPvd = NULL;
Particula* p = NULL;
PxDefaultCpuDispatcher* gDispatcher = NULL;
PxScene* gScene = NULL;
ContactReportCallback gContactReportCallback;
/////////////////////////////////////////////////////////////
//Ejes
RenderItem* Xaxis;
RenderItem* Yaxis;
RenderItem* Zaxis;
RenderItem* center;
//PARTICULAS
std::vector<Proyectil*> pistolas;
//delta
double deltaTime = 0.016; // inicialización global
//sistema
SistemaParticulas* sistema = nullptr;
//FUERZAS
Viento* viento = nullptr;
Torbellino* torbellino = nullptr;
Explosion* explosion = nullptr;
TunelViento* tunelViento = nullptr;
FuenteAgua* fuenteAgua = nullptr;
Muelle* muelle = nullptr;
Flotacion* flotacion = nullptr;
//SOLIDOS
 Pelota* pelota = nullptr;//borrarlaaaa
 SistemaSolidos* sistemaSolidos = nullptr;


//creacion de ejes
void creacionEjes() {

	//ejes
	Vector3D X(1, 0, 0);
	Vector3D Y(0, 1, 0);
	Vector3D Z(0, 0, 1);
	Vector3D centro(0, 0, 0);

	Vector3D eX = X.numeroPorVec(4);
	Vector3D eY = Y.numeroPorVec(4);
	Vector3D eZ = Z.numeroPorVec(4);

	Xaxis = new RenderItem(CreateShape(PxSphereGeometry(1.0f)), new PxTransform(PxVec3(eX.getX(), eX.getY(), eX.getZ())), Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	Yaxis = new RenderItem(CreateShape(PxSphereGeometry(1.0f)), new PxTransform(PxVec3(eY.getX(), eY.getY(), eY.getZ())), Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	Zaxis = new RenderItem(CreateShape(PxSphereGeometry(1.0f)), new PxTransform(PxVec3(eZ.getX(), eZ.getY(), eZ.getZ())), Vector4(0.0f, 0.0f, 1.0f, 1.0f));
	center = new RenderItem(CreateShape(PxSphereGeometry(1.0f)), new PxTransform(PxVec3(centro.getX(), centro.getY(), centro.getZ())), Vector4(1.0f, 1.0f, 1.0f, 1.0f));

}


// Initialize physics engine
void initPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);
	physx::PxTransform trans(physx::PxVec3(5.0f, 5.0f, 0.0f));

	//suelooo
	PxMaterial* mat = gPhysics->createMaterial(0.5f, 0.5f, 0.2f); // staticFriction, dynamicFriction, restitution
	PxRigidStatic* suelo = gPhysics->createRigidStatic(
		PxTransform(PxVec3(480.0f, 0.0f, 250.0f))
	);
	PxShape* shapeSuelo = gPhysics->createShape(
		PxBoxGeometry(500.0f, 1.0f, 500.0f), *mat
	);
	suelo->attachShape(*shapeSuelo);
	gScene->addActor(*suelo);

	// Render, CUIDADO BORRAR!!!
	new RenderItem(shapeSuelo, suelo, PxVec4(0.0f, 0.3f, 1.0f, 1.0f));


	//ejes
	creacionEjes();

	//sistema
	sistema = new SistemaParticulas();

	//Viento
	viento = new Viento(200.0f, Vector3D(1, 0, 0), 0.01f);
	sistema->addFuerza(viento);

	//torbellino
	torbellino = new Torbellino(Vector3D(0, 0, 0), 50.0f, 20.0f, 30.0f);
	sistema->addFuerza(torbellino);

	//Explosion
	explosion = new Explosion(500.0f, 10.0f, Vector3D(2, 5, 0), 5.0f, 20.0f);
	sistema->addFuerza(explosion);

	//tunel
	tunelViento = new TunelViento(Vector3D(20, 0, 20), 300.0f, 15.0f);
	sistema->addFuerza(tunelViento);

	//muelle
	//Particula* f1 = new Particula(Vector3D(0, 0, 0), Vector3D(0, 1, 0), Vector3D(0, -10, 0));//se borran al destruir el sistema
	//sistema->addParticula(f1);
	//f1->permitirFuerza("muelle");
	//Particula* f2 = new Particula(Vector3D(0, 0, 0), Vector3D(0, 15, 0), Vector3D(0, -10, 0), Vector4(1.0,0,0,1));
	//f2->permitirFuerza("muelle");
	//sistema->addParticula(f2);
	//float restLength = (f2->getPos() - f1->getPos()).modulo();//union falsa entre las dos
	//muelle = new Muelle( { 0,1,0 }, 10, restLength);
	//sistema->addFuerza(muelle);

	////flotacion
	//float half = 1.0f;
	//float alturaCubo = 2.0f;//altura real
	//float volumenCubo = 1.0f;//2x2x2
	//float masa = 5.0f;
	//Particula* cuboAgua = new Particula(Vector3D(0, 0, 0), Vector3D(0, 0, 0), Vector3D(0, -10, 0), Vector4(0, 1.0, 0, 1)
	//,new physx::PxBoxGeometry(half, half, half), 1.0f, masa);
	//cuboAgua->permitirFuerza("flotacion");
	//sistema->addParticula(cuboAgua);

	//flotacion = new Flotacion(Vector3D(0, 0, 0), alturaCubo, volumenCubo, 10.0f);
	//sistema->addFuerza(flotacion);


	sistemaSolidos = new SistemaSolidos();
	//Pelotaa
	pelota = new Pelota(
		gPhysics,
		gScene,
		gMaterial,
		Vector3D(30, 5, 30),    // posición inicial
		2.0f,                 // radio
		Vector4(1.0f, 0.0f, 0.0f, 1.0f)   // color celeste
	);
	// Tamaño del cubo (2x2x2)
	Vector3D tam(2.0f, 2.0f, 2.0f);

	// Posición del cubo
	Vector3D pos(0.0f, 02.0f, 0.0f);

	// Color del render (RGBA)
	PxVec4 color(1.0f, 0.5f, 0.5f, 1.0f);

	// Crear cubo estático
	Solido* cubillo = new Solido(gPhysics, gScene, mat, pos, tam, color, Solido::Tipo::Estatico);
	sistemaSolidos->addSolido(cubillo);

	
	Mapa* mapa = new Mapa("C:/Users/Portatil/Documents/GitHub/SimulacionFisicaVideojuegos/mapa.txt"
		, sistemaSolidos, gPhysics, gScene, gMaterial);
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	deltaTime = t; // actualizar cada frame
	PX_UNUSED(interactive);

	gScene->simulate(t);
	for (auto p : pistolas)
	{
		p->integrate(t);
	}

	if (sistema) {
		sistema->actualizar(deltaTime);//emite y actualiza constantemente
		sistema->mata(deltaTime);
	}
	if(sistemaSolidos) sistemaSolidos->actualizar(deltaTime);
	if (pelota) {
		pelota->update(t);
		if (tunelViento) tunelViento->updateFuerzas(pelota, t);
	}

	gScene->fetchResults(true);
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	gScene->release();
	gDispatcher->release();
	// -----------------------------------------------------
	gPhysics->release();
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();

	gFoundation->release();

	//borrar ejes
	DeregisterRenderItem(Xaxis);
	DeregisterRenderItem(Yaxis);
	DeregisterRenderItem(Zaxis);
	DeregisterRenderItem(center);

	//borrar PARTICULA!!!!
	for (auto pa : pistolas)
	{
		delete pa;
	}
	//borrar sistema
	if (sistema) {
		delete sistema;
		sistema = nullptr;
	}
	
}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

	switch (toupper(key))
	{
	case ' ':
	{
		auto eyePx = GetCamera()->getEye();
		auto dirPx = GetCamera()->getDir();

		Vector3D eye(eyePx.x, eyePx.y, eyePx.z);
		Vector3D dir(dirPx.x, dirPx.y, dirPx.z);

		//pistolas.push_back(new Proyectil(eye, dir, 600.0f, 60.0f, 0.6f));//bala
		pistolas.push_back(new Proyectil(eye, dir, 20.0f, 15.0f, 0.65f));//bola pesada

		break;
	}
	case 'M': {
		auto eyePx = GetCamera()->getEye();
		auto dirPx = GetCamera()->getDir();

		Vector3D eye(eyePx.x, eyePx.y, eyePx.z);
		Vector3D dir(dirPx.x, dirPx.y, dirPx.z);
		//se dispara desde la camara hacia donde mira
		physx::PxBoxGeometry disco(0.3f, 0.05f, 0.3f);
		pistolas.push_back(new Proyectil(eye, dir, 600.0f, 60.0f, 0.6f, Vector4(1.0f, 0, 0, 1.0f), &disco));
		break;
	}
	case 'B': {

		if (sistema) {
			Vector3D posicionFuente(20.0f, 0.0f, 20.0f);
			Vector3D direccionFuente(0.0f, 1.0f, 0.0f);
			float velocidadParticula = 20.0f;
			float tasaEmision = 50.0f;

			if (!fuenteAgua) {
				Particula* p = new Particula({ 0,0,0 }, { 0,0,0 }, Vector3D(0,-10,0));
				fuenteAgua = new FuenteAgua(p, posicionFuente, direccionFuente, velocidadParticula, tasaEmision, 2.0f);
				sistema->addFuente(fuenteAgua);
				fuenteAgua->emitir(deltaTime);
			}
			else
			{
				sistema->eliminarFuente(fuenteAgua);
				fuenteAgua = nullptr;
			}
		}
		break;
	}
	case 'E': {
		if (explosion)
			explosion->toggle();  // activa la explosión
		break;
	}
	case 'T': {
		if (tunelViento)
			tunelViento->toggle();
		break;
	}
	case 'V': {
		if (viento)
			viento->toggle();
		break;
	}
	case 'H': {
		if (torbellino)
			torbellino->toggle();
		break;
	}
	case 'P': {
		if (muelle)
			muelle->toggle();
		break;
	}
	case 'I': {
		if (flotacion)
			flotacion->toggle();
		break;
	}
	case 'F': {
		Particula* p = new Particula({ 0,0,0 }, { 0,0,0 }, Vector3D(0, -10, 0), Vector4(1.0f,0.4f,0.4f,1.0f));

		FuenteFuegosArtificiales* fuegos = new FuenteFuegosArtificiales(p, Vector3D(0, 0, 0), 30.0f, 2.0f, 2.0f);
		sistema->addFuente(fuegos);
		fuegos->emitir(deltaTime);

		break;
	}
	case 'L': {
		Particula* p = new Particula({ 0,0,0 }, { 0,0,0 }, Vector3D(0, -9.8f, 0));

		FuenteFuego* fuego = new FuenteFuego(p, Vector3D(0.0f, 0.0f, 0.0f), 10.0f, 5.0f, 40.0f, 2.0f);
		sistema->addFuente(fuego);
		fuego->emitir(deltaTime);
		break;
	}
	case 'R': {
		//esto es para resetear la posicion de mi bola de golf, en principio solo habrá una asi que lo del vector sobra
		//tambien quiero que si se cae al vacio se reposicione
		/*for (auto p : pistolas) {
			p->reset(p->getPosIni()+Vector3D(-3,-2,-3));
		}*/
		pelota->reset();
		break;
	}
	default:
		break;
	}
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
}


int main(int, const char* const*)
{
#ifndef OFFLINE_EXECUTION 
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for (PxU32 i = 0; i < frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif

	return 0;
}