#include <vector>

#include "PxPhysicsAPI.h"

#include "core.hpp"
#include "RenderUtils.hpp"
#include "globales.h"



using namespace physx;

extern void initPhysics(bool interactive);
extern void stepPhysics(bool interactive, double t);
extern void cleanupPhysics(bool interactive);
extern void keyPress(unsigned char key, const PxTransform& camera);
extern PxPhysics* gPhysics;
extern PxMaterial* gMaterial;

std::vector<const RenderItem*> gRenderItems;

double PCFreq = 0.0;
__int64 CounterStart = 0;
__int64 CounterLast = 0;

void StartCounter()
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
		return;

	PCFreq = double(li.QuadPart) /*/ 1000.0*/;

	QueryPerformanceCounter(&li);
	CounterStart = li.QuadPart;
	CounterLast = CounterStart;
}

double GetCounter()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	double t = double(li.QuadPart - CounterLast) / PCFreq;
	CounterLast = li.QuadPart;
	return t;
}

namespace
{
	Camera* sCamera;

	void motionCallback(int x, int y)
	{
		if (!pelota)
			return;

		if (pelota->estaArrastrando())
		{
			// Actualiza el punto actual para la línea
			Vector3D world = pelota->screenToWorld(x, y);
			pelota->setpuntoA(world);
		}
		else
		{
			sCamera->handleMotion(x, y);
		}
	}

	void keyboardCallback(unsigned char key, int x, int y)
	{
		if (key == 27)
			exit(0);

		/*if (!sCamera->handleKey(key, x, y))
			keyPress(key, sCamera->getTransform());*/
		if (!pelota || !pelota->estaArrastrando())
			sCamera->handleKey(key, x, y);

		keyPress(key, sCamera->getTransform());
	}

	void mouseCallback(int button, int state, int x, int y)
	{
		
		sCamera->handleMouse(button, state, x, y); // solo mueve la cámara si NO arrastras la pelot
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_DOWN && pelota)
			{
				if (pelota->estaParada()) //en el otro lado devuelvo siemre true!!!! ACORDARSE
				{
					// Detectar si el clic está sobre la pelota
					PxVec3 origen, direccion;
					if (GetCamera()->getPickRay(x, y, origen, direccion))
					{
						PxRaycastBuffer hit;
						if (pelota->raycast(origen, direccion, hit))  // Método que verificas si golpea la pelota
						{
							// Solo si se hace clic sobre la pelota
							pelota->setArrastrando(true);
							pelota->setpuntoI(pelota->getPos());  // Punto inicial = posición de la pelota
							pelota->setpuntoA(pelota->getPos());  // Punto actual empieza igual
						}
					}
				}
			}
			else if (state == GLUT_UP && pelota && pelota->estaArrastrando())
			{
				pelota->soltarArrastre();
			}
		}

	}

	void idleCallback()
	{
		glutPostRedisplay();

	}

	float stepTime = 0.0f;
	//#define FIXED_STEP

	void renderCallback()
	{
		double t = GetCounter();

		// Actualiza la física
		stepPhysics(true, t);

		// Inicia el renderizado
		startRender(sCamera->getEye(), sCamera->getDir());

		// Dibuja los objetos
		for (auto it = gRenderItems.begin(); it != gRenderItems.end(); ++it)
		{
			const RenderItem* obj = (*it);
			auto objTransform = obj->transform;
			if (!objTransform)
			{
				auto actor = obj->actor;
				if (actor)
					renderShape(*obj->shape, actor->getGlobalPose(), obj->color);
				continue;
			}
			renderShape(*obj->shape, objTransform ? *objTransform : PxTransform(PxIdentity), obj->color);
		}

		// Dibuja la línea de arrastre si se está arrastrando la pelota
		if (pelota && pelota->estaArrastrando())
			drawLine(pelota->getpuntoI(), pelota->getpuntoA());

		// Termina el renderizado
		finishRender();
//#ifdef FIXED_STEP
//		if (t < (1.0f / 30.0f))
//		{
//			fprintf(stderr, "Time: %f\n", stepTime);
//			stepTime += t;
//		}
//		else
//			stepTime = 1.0f / 30.0f;
//
//		if (stepTime >= (1.0f / 30.0f))
//		{
//			stepPhysics(true, stepTime);
//			stepTime = 0.0f;
//		}
//#else
//		stepPhysics(true, t);
//#endif
//
//		startRender(sCamera->getEye(), sCamera->getDir());
//
//		//fprintf(stderr, "Num Render Items: %d\n", static_cast<int>(gRenderItems.size()));
//		for (auto it = gRenderItems.begin(); it != gRenderItems.end(); ++it)
//		{
//			const RenderItem* obj = (*it);
//			auto objTransform = obj->transform;
//			if (!objTransform)
//			{
//				auto actor = obj->actor;
//				if (actor)
//				{
//					renderShape(*obj->shape, actor->getGlobalPose(), obj->color);
//					continue;
//				}
//			}
//			renderShape(*obj->shape, objTransform ? *objTransform : physx::PxTransform(PxIdentity), obj->color);
//		}
//
//		//PxScene* scene;
//		//PxGetPhysics().getScenes(&scene, 1);
//		//PxU32 nbActors = scene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);
//		//if (nbActors)
//		//{
//		//	std::vector<PxRigidActor*> actors(nbActors);
//		//	scene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC, reinterpret_cast<PxActor**>(&actors[0]), nbActors);
//		//	renderActors(&actors[0], static_cast<PxU32>(actors.size()), true, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
//		//}
//
//		finishRender();
	}

	void exitCallback(void)
	{
		delete sCamera;
		cleanupPhysics(true);
	}
}

void renderLoop()
{
	StartCounter();
	sCamera = new Camera(PxVec3(-50.0f, 50.0f, 40.0f), PxVec3(-0.6f, -0.2f, -0.7f));

	setupDefaultWindow("Simulacion Fisica Videojuegos");
	setupDefaultRenderState();

	glutIdleFunc(idleCallback);
	glutDisplayFunc(renderCallback);
	glutKeyboardFunc(keyboardCallback);
	glutMouseFunc(mouseCallback);
	glutMotionFunc(motionCallback);

	motionCallback(0, 0);

	atexit(exitCallback);

	initPhysics(true);
	glutMainLoop();
}

void RegisterRenderItem(const RenderItem* _item)
{
	gRenderItems.push_back(_item);
}

void DeregisterRenderItem(const RenderItem* _item)
{
	auto it = find(gRenderItems.begin(), gRenderItems.end(), _item);
	gRenderItems.erase(it);
}

double GetLastTime()
{
	double t = double(CounterLast - CounterStart) / PCFreq;
	return t;
}

Camera* GetCamera()
{
	return sCamera;
}

PxShape* CreateShape(const PxGeometry& geo, const PxMaterial* mat)
{
	if (mat == nullptr)
		mat = gMaterial; // Default material

	PxShape* shape = gPhysics->createShape(geo, *mat);
	return shape;
}

void drawLine(const Vector3D& start, const Vector3D& end)
{
	glDisable(GL_LIGHTING);
	glLineWidth(4.0f);
	glColor3f(1.0f, 0.0f, 0.0f); // rojo
	glBegin(GL_LINES);
	glVertex3f(start.getX(), start.getY(), start.getZ());
	glVertex3f(end.getX(), end.getY(), end.getZ());
	glEnd();
	glEnable(GL_LIGHTING);
}
