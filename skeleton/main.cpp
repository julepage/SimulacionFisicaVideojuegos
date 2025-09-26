#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"
#include "Vector3D.h"

#include <iostream>

std::string display_text = "This is a test";


using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation* gFoundation = NULL;
PxPhysics* gPhysics = NULL;


PxMaterial* gMaterial = NULL;

PxPvd* gPvd = NULL;

PxDefaultCpuDispatcher* gDispatcher = NULL;
PxScene* gScene = NULL;
ContactReportCallback gContactReportCallback;


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

	Vector3D X(1, 0, 0);
	Vector3D Y(0, 1, 0);
	Vector3D Z(0, 0, 1);
	Vector3D centro(0, 0, 0);

	Vector3D eX = X.numeroPorVec(4);
	Vector3D eY = Y.numeroPorVec(4);
	Vector3D eZ = Z.numeroPorVec(4);

	PxTransform* ejeX = new PxTransform(PxVec3(eX.getX(), eX.getY(), eX.getZ()));
	PxTransform* ejeY = new PxTransform(PxVec3(eY.getX(), eY.getY(), eY.getZ()));
	PxTransform* ejeZ = new PxTransform(PxVec3(eZ.getX(), eZ.getY(), eZ.getZ()));
	PxTransform* c = new PxTransform(PxVec3(centro.getX(), centro.getY(), centro.getZ()));

	RenderItem* Xaxis = new RenderItem(CreateShape(PxSphereGeometry(1.0f)), ejeX, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	RenderItem* Yaxis = new RenderItem(CreateShape(PxSphereGeometry(1.0f)), ejeY, Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	RenderItem* Zaxis = new RenderItem(CreateShape(PxSphereGeometry(1.0f)), ejeZ, Vector4(0.0f, 0.0f, 1.0f, 1.0f));
	RenderItem* center = new RenderItem(CreateShape(PxSphereGeometry(1.0f)), c, Vector4(1.0f, 1.0f, 1.0f, 1.0f));


}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	gScene->simulate(t);
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