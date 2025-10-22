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
#include "SistemaParticulas.h"

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
//Fuente particulas
FuenteParticulas* miFuente = nullptr;
//sistema
SistemaParticulas* sistema;


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

	//ejes
	creacionEjes();

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

	if (miFuente)
	{
		miFuente->emitir(deltaTime);
		miFuente->actualizar(t); 
		sistema->mata(t);
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
	for (auto p : pistolas)
	{
		delete p;
	}
	//borrar fuente
	if (miFuente) {
		delete miFuente;
		miFuente = nullptr;
		delete sistema;
	}
}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

	switch (toupper(key))
	{
		//case 'B': break;
		//case ' ':	break;
	case ' ':
	{
		auto eyePx = GetCamera()->getEye();  // PxVec3
		auto dirPx = GetCamera()->getDir();  // PxVec3

		Vector3D eye(eyePx.x, eyePx.y, eyePx.z);
		Vector3D dir(dirPx.x, dirPx.y, dirPx.z);

		pistolas.push_back(new Proyectil(eye, dir, 600.0f, 60.0f, 0.6f));

		break;
	}
	case 'B': {

		if (!miFuente) {
			Vector3D posicionFuente(0.0f, 0.0f, 0.0f);
			Vector3D direccionFuente(0.0f, 1.0f, 0.0f);
			float velocidadParticula = 20.0f;
			float tasaEmision = 50.0f;

			miFuente = new FuenteParticulas(posicionFuente, direccionFuente, velocidadParticula, tasaEmision);
			sistema = new SistemaParticulas(*miFuente);
		}

		miFuente->emitir(deltaTime);


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