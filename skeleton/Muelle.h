#pragma once
#include "GeneradorFuerzas.h"
#include "SistemaSolidos.h"
using namespace physx;

class Muelle: public GeneradorFuerzas
{
public:
	Muelle( Vector3D pos, float k, float longitud): pos(pos), k(k), longitud(longitud) {
		//Creo un cuadrado en la pos un poco mas atrás como ancla
		ancla = new RenderItem(CreateShape(PxBoxGeometry(5.0f, 2.0f, 20.0f)), new PxTransform(PxVec3(pos.getX(), pos.getY(), pos.getZ())), Vector4(0.4f, 0.2f, 0.0f, 1.0f));
		activa = true;
	};

    Muelle(PxPhysics* physics, PxScene* scene, PxMaterial* mat, Vector3D pos, float altura, SistemaSolidos* sistema)
    {
        this->pos = pos;
        this->altura = altura;
        ancla = new RenderItem(CreateShape(PxBoxGeometry(5.0f, 2.0f, 20.0f)), new PxTransform(PxVec3(pos.getX(), pos.getY(), pos.getZ())), Vector4(0.4f, 0.2f, 0.0f, 1.0f));
        activa = true;
        // Crear el cubo que sube y baja como puerta
        Vector3D tam(5.0f, 30.0f, 18.0f); // ancho x altura x profundidad
        
        Vector3D puertaPos(pos.getX(), pos.getY(), pos.getZ()); // empieza en la misma Y que el ancla
        puerta = new Solido(physics, scene, mat, puertaPos, tam, PxVec4(0.6f, 0.3f, 0.f, 1.f), Solido::Tipo::Dinamico);
        this->longitud = 5.0f; // distancia de reposo pequeña para que la fuerza inicial actúe
        sistema->addSolido(puerta);
        //puerta->getCuerpoDinamico()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
        PxRigidDynamic* cuerpo = puerta->getCuerpoDinamico();

        //// Establecer la densidad deseada (en kg/m³)
        //float densidad = 1000.0f; // ejemplo: 1000 kg/m³

        // Actualiza la masa y la inercia según la densidad
        //PxRigidBodyExt::updateMassAndInertia(*cuerpo, densidad);
        cuerpo->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, true);
        cuerpo->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, true);
        cuerpo->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, true);
    }

	virtual ~Muelle() { if (ancla != nullptr) { DeregisterRenderItem(ancla);  ancla = nullptr; } };
	void updateFuerzas(Particula* p, double t) override; // por defecto no hace nada pero es por si necesito actualizar por frmae y no por particulas como en la explosion
	void toggle() override;

    void update(float deltaTime);

protected:
	float k;//cte elastica muelle
	Vector3D pos;//posicion inicial
	float longitud;//longitud del objeto
	RenderItem* ancla;

    Solido* puerta = nullptr;
    bool haciaArriba;
    float velocidad;
    float altura;
    SistemaSolidos* sistema;
    PxPhysics* gPhysics;
    PxScene* gScene;
    PxMaterial* gMaterial;

};

