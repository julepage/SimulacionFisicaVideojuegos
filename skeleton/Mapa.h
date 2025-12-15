#pragma once
#include <vector>
#include <string>
#include "Vector3D.h"
#include "Solido.h"
#include "SistemaSolidos.h"
#include <PxPhysicsAPI.h>

using namespace physx;

class Mapa {
private:
    SistemaSolidos* sistemaSolidos; // sistema que maneja los sólidos
    PxPhysics* gPhysics;
    PxScene* gScene;
    PxMaterial* gMaterial;

public:
    // Constructor: lee un archivo de mapa y genera los sólidos
    Mapa(const std::string& filename,
        SistemaSolidos* sistema,
        PxPhysics* physics,
        PxScene* scene,
        PxMaterial* mat);

    ~Mapa() = default;
};
