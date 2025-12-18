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
FuenteFuego* fuenteFuego = nullptr;
Muelle* muelle = nullptr;
Flotacion* flotacion = nullptr;
//SOLIDOS
Pelota* pelota = nullptr;//borrarlaaaa
SistemaSolidos* sistemaSolidos = nullptr;
Muelle* muellePuerta = nullptr;
double tiempoMax = 150.0;   // 5 minutos/2
double tiempoRestante = tiempoMax;
std::string display_text = " ";
Solido* suelo = nullptr;

bool jugando = true;

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


void lanzarFuegosArtificiales()
{
	// Crear partícula base
	Particula* p = new Particula({ 0,0,0 }, { 0,0,0 }, Vector3D(0, -9.8f, 0));

	// Posición inicial de los fuegos artificiales
	Vector3D posFuegos(450.0f, 5.0f, 180.0f); // ajustar según escena

	// Crear fuente de fuegos artificiales
	FuenteFuegosArtificiales* fuegos = new FuenteFuegosArtificiales(p, posFuegos,
		30.0f, // vel
		3.0f,  // tasa
		7.0f); // dist
	sistema->addFuente(fuegos);

	// Emitir inmediatamente
	fuegos->emitir(deltaTime);
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

	gMaterial = gPhysics->createMaterial(0.3f, 0.3f, 0.6f);

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);
	physx::PxTransform trans(physx::PxVec3(5.0f, 5.0f, 0.0f));
	PxMaterial* mat = gPhysics->createMaterial(0.5f, 0.5f, 0.2f); // staticFriction, dynamicFriction, restitution

	//ejes
	//creacionEjes();

	//sistema
	sistema = new SistemaParticulas();

	//FUENTES DE AGUA
	Vector3D posicionFuente(200.0f, -15.0f, 40.0f);
	Vector3D direccionFuente(0.0f, 1.0f, 0.0f);
	float velocidadParticula = 20.0f;
	float tasaEmision = 50.0f;

	Particula* p = new Particula({ 0,0,0 }, { 0,0,0 }, Vector3D(0, -10, 0),Vector4(0.0f, 0.4f, 1.0f, 1.0f),nullptr, 2.0, 1.0, 9.8,1.0f);
	fuenteAgua = new FuenteAgua(p, posicionFuente, direccionFuente, velocidadParticula, tasaEmision, 2.0f);
	sistema->addFuente(fuenteAgua);
	fuenteAgua->emitir(deltaTime);
	Particula* d = new Particula({ 0,0,0 }, { 0,0,0 }, Vector3D(0, -10, 0), PxVec4(0.0f, 0.3f, 1.0f, 1.0f));
	FuenteAgua* fuenteAgua2 = new FuenteAgua(d, { 80,0,250 }, direccionFuente, velocidadParticula, tasaEmision, 2.0f);
	sistema->addFuente(fuenteAgua2);
	fuenteAgua2->emitir(deltaTime);

	//LLAMAS
	Particula* a = new Particula({ 0,0,0 }, { 0,0,0 }, Vector3D(0, -9.8f, 0));
	fuenteFuego = new FuenteFuego(a, Vector3D(660.0f, 5.0f, 5.0f), 10.0f, 5.0f, 150.0f, 2.0f);
	sistema->addFuente(fuenteFuego);
	fuenteFuego->emitir(deltaTime);
	Particula* b = new Particula({ 0,0,0 }, { 0,0,0 }, Vector3D(0, -9.8f, 0));
	FuenteFuego* fuenteFuego2 = new FuenteFuego(b, Vector3D(640.0f, 5.0f, 75.0f), 10.0f, 5.0f, 120.0f, 2.0f);
	sistema->addFuente(fuenteFuego2);
	fuenteFuego2->emitir(deltaTime);
	//Viento
	viento = new Viento({ 400, 10, 40 }, 4000.0f, Vector3D(1, 0, 0), 0.01f);
	sistema->addFuerza(viento);
	viento->setActiva(true);

	//torbellino
	torbellino = new Torbellino({ 80,0,250 }, 50.0f, 20.0f, 30.0f);
	sistema->addFuerza(torbellino);
	torbellino->setActiva(true);

	//Explosion
	explosion = new Explosion(500.0f, 10.0f, Vector3D(2, 5, 0), 5.0f, 20.0f);
	sistema->addFuerza(explosion);

	//tunel
	tunelViento = new TunelViento(Vector3D(200, 0, 40), 300.0f, 15.0f);
	sistema->addFuerza(tunelViento);
	tunelViento->setActiva(true);

	//muelle
	//Particula* f1 = new Particula(Vector3D(450, 50, 40), Vector3D(0, 1, 0), Vector3D(0, -10, 0));//se borran al destruir el sistema
	//sistema->addParticula(f1);
	//f1->permitirFuerza("muelle");
	//Particula* f2 = new Particula(Vector3D(450, 50, 40), Vector3D(0, 15, 0), Vector3D(0, -10, 0), Vector4(1.0,0,0,1));
	//f2->permitirFuerza("muelle");
	//sistema->addParticula(f2);
	//float restLength = (f2->getPos() - f1->getPos()).modulo();//union falsa entre las dos
	//muelle = new Muelle( { 450,50,40 }, 10, restLength);
	//sistema->addFuerza(muelle);

	//flotacion
	float half = 1.0f;
	float alturaCubo = 2.0f;//altura real
	float volumenCubo = 1.0f;//2x2x2
	float masa = 5.0f;
	Particula* cuboAgua = new Particula(Vector3D(80, 5, 260), Vector3D(0, 0, 0), Vector3D(0, -10, 0), Vector4(0.0, 0.0, 0.0, 1)
	,new physx::PxBoxGeometry(15.0f, 15.0f, 40.0f), 1.0f, masa);
	cuboAgua->permitirFuerza("flotacion");
	sistema->addParticula(cuboAgua);

	flotacion = new Flotacion(Vector3D(0, -5, 0), alturaCubo, volumenCubo, 10.0f);
	sistema->addFuerza(flotacion);


	sistemaSolidos = new SistemaSolidos();

	//suelo
	Vector3D tamSuelo(1000.0f, 5.0f, 1000.0f);
	Vector3D posSuelo(250.0f, -5.0f, 250.0f);
	PxVec4 colorSuelo(0.0f, 0.3f, 1.0f, 1.0f);
	suelo = new Solido(gPhysics,gScene,gMaterial,posSuelo,tamSuelo,colorSuelo,Solido::Tipo::Estatico);
	sistemaSolidos->addSolido(suelo);

	//Pelotaa
	pelota = new Pelota(gPhysics,gScene,gMaterial,Vector3D(40, 5, 40),  2.0f, Vector4(1.0f, 0.0f, 0.0f, 1.0f) );
	sistemaSolidos->addSolido(pelota);

	//mapa
	Mapa* mapa = new Mapa("C:/Users/Portatil/Documents/GitHub/SimulacionFisicaVideojuegos/mapa.txt",sistemaSolidos, gPhysics, gScene, gMaterial);

	//puerta
	Vector3D posAncla(450, 30, 40);     
	float alturaPuerta = 20.f;            
	float k = 10.f;                        
	float longitudReposo = 5.f;            

	muellePuerta = new Muelle(gPhysics, gScene, gMaterial, posAncla, alturaPuerta, sistemaSolidos);
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	deltaTime = t; // actualizar cada frame
	PX_UNUSED(interactive);

	gScene->simulate(t);

	if (jugando) {
		//CAMARA
		GetCamera()->update(t);

		//TIEMPO
		tiempoRestante -= t;
		if (tiempoRestante <= 0.0) {
			tiempoRestante = 0.0;
			display_text = "GAME OVER";
			jugando = false;
		}
		else {
			int tiempoEntero = static_cast<int>(tiempoRestante);
			int minutos = tiempoEntero / 60;
			int segundos = tiempoEntero % 60;
			display_text = "TIEMPO RESTANTE: " +
				std::to_string(minutos) + ":" +
				(segundos < 10 ? "0" : "") + std::to_string(segundos);

		}
		if (tiempoRestante > 0 && sistemaSolidos) {
			if (sistemaSolidos->victoria()) {
				jugando = false;
				display_text = "YOU WIN!";
				std::cout << "ganasteeeeeeeeeeeeeeeee";
				lanzarFuegosArtificiales();

				Particula* x = new Particula({ 0,0,0 }, { 0,0,0 }, Vector3D(0, -10, 0), PxVec4(1.0f, 0.0f, 1.0f, 1.0f));
				FuenteAgua* fuenteAgua3 = new FuenteAgua(x, { 550,5,200 }, {0,1,0}, 20.0f, 50, 2.0f);
				sistema->addFuente(fuenteAgua3);
				fuenteAgua3->emitir(deltaTime);
				Particula* y = new Particula({ 0,0,0 }, { 0,0,0 }, Vector3D(0, -10, 0), PxVec4(1.0f, 0.0f, 1.0f, 1.0f));
				FuenteAgua* fuenteAgua4 = new FuenteAgua(y, { 550,5,160 }, {0,1,0}, 20.0f, 50, 2.0f);
				sistema->addFuente(fuenteAgua4);
				fuenteAgua4->emitir(deltaTime);
			}
		}
		//JUEGO
		for (auto p : pistolas)
		{
			p->integrate(t);
		}
	}
	else {
		pelota->reset();
	}
	if (sistema) {
		sistema->actualizar(deltaTime);//emite y actualiza constantemente
		sistema->mata(deltaTime);
	}
	if (sistemaSolidos) sistemaSolidos->actualizar(deltaTime);
	if (pelota) {
		if (sistemaSolidos->intersecan(pelota, suelo)) {
			pelota->reset();
			GetCamera()->resetCamera();

		}
		if (tunelViento) tunelViento->updateFuerzas(pelota, t);
		if (fuenteFuego && fuenteFuego->colisionConPelota())
		{
			pelota->reset();
			GetCamera()->resetCamera();
		}

		if (viento) viento->updateFuerzas(pelota, t);
	}
	if (muellePuerta) muellePuerta->update(deltaTime);
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
	/*DeregisterRenderItem(Xaxis);
	DeregisterRenderItem(Yaxis);
	DeregisterRenderItem(Zaxis);
	DeregisterRenderItem(center);*/

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

		if (fuenteAgua) {
		}
		sistema->eliminarFuente(fuenteAgua);
		fuenteAgua = nullptr;
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
		Particula* p = new Particula({ 0,0,0 }, { 0,0,0 }, Vector3D(0, -10, 0), Vector4(1.0f, 0.4f, 0.4f, 1.0f));
		FuenteFuegosArtificiales* fuegos = new FuenteFuegosArtificiales(p, Vector3D(0, 0, 0), 40.0f, 2.0f, 2.0f);
		sistema->addFuente(fuegos);
		fuegos->emitir(deltaTime);

		break;
	}
	case 'R': {
		pelota->reset();
		GetCamera()->resetCamera();
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