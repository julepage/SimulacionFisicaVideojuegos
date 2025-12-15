#pragma once
#include "RenderUtils.hpp"
#include <PxPhysicsAPI.h>
#include "Vector3D.h"

using namespace physx;

class Pelota
{
public:
    Pelota(PxPhysics* physics, PxScene* scene, PxMaterial* mat,
        Vector3D posInicial, float radio, Vector4 colorBola);

    ~Pelota();

    // Arrastre
    void comenzarArrastre();
    void actualizarArrastre(int mouseX, int mouseY); // ya no mueve la pelota
    void soltarArrastre();
    Vector3D screenToWorld(int x, int y);
    bool raycast(const PxVec3& origen, const PxVec3& direccion, PxRaycastBuffer& hit);
    void Pelota::aplicarFuerza(const Vector3D& fuerza, PxForceMode::Enum modo = PxForceMode::eIMPULSE)
    {
        if (!cuerpo) return;
        cuerpo->wakeUp();
        PxVec3 f(fuerza.getX(), fuerza.getY(), fuerza.getZ());
        cuerpo->addForce(f, modo);
    }

    // Getters
    Vector3D getPosIni() const noexcept { return posI; }
    Vector3D getPos() const;
    bool estaArrastrando() const { return arrastrando; }
    Vector3D getpuntoI() { return puntoInicio; }
    Vector3D getpuntoA() { return puntoActual; }
    bool estaParada(float umbral = 1.0f) const;

    void update(float deltaTime);

    // Setters
    void setArrastrando(bool s) { arrastrando = s; }
    void setpuntoI(Vector3D p) { puntoInicio = p; }
    void setpuntoA(Vector3D p) { puntoActual = p; }

    void reset();


protected:
    PxRigidDynamic* cuerpo = nullptr;
    RenderItem* render = nullptr;
    PxScene* scene;
    Vector3D posI;
    Vector4 color;

    bool arrastrando = false;
    bool movida = false; // indica si la pelota ha empezado a moverse alguna vez

    Vector3D puntoInicio; // punto donde empieza el arrastre
    Vector3D puntoActual; // posición actual del ratón proyectada al mundo
};
