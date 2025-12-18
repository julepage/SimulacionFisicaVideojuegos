#pragma once
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"
#include "Vector3D.h"

using namespace physx;

class Solido {
public:
    enum class Tipo { Estatico, Dinamico };
    enum class TipoEspecial { Ninguno, Pelota, Meta }; // NUEVO

    // Constructor para cubo con rotación opcional
    Solido(PxPhysics* physics, PxScene* scene, PxMaterial* mat,
        const Vector3D& pos, const Vector3D& tam, const PxVec4& color,
        Tipo tipo = Tipo::Dinamico,
        const PxQuat& rotacion = PxQuat(PxIdentity),
        TipoEspecial te = TipoEspecial::Ninguno) // NUEVO
        : scene(scene), tipo(tipo), tipoEspecial(te)
    {
        PxVec3 posPx(pos.getX(), pos.getY(), pos.getZ());
        PxVec3 tamPx(tam.getX(), tam.getY(), tam.getZ());

        PxShape* shape = nullptr;

        if (tipo == Tipo::Dinamico) {
            cuerpoD = physics->createRigidDynamic(PxTransform(posPx, rotacion));
            shape = physics->createShape(PxBoxGeometry(tamPx / 2.0f), *mat, true);
            cuerpoD->attachShape(*shape);
            PxRigidBodyExt::updateMassAndInertia(*cuerpoD, 1.0f);
            scene->addActor(*cuerpoD);

            // Asignamos userData para callbacks
            cuerpoD->userData = this;

            render = new RenderItem(shape, cuerpoD, color);
        }
        else {
            cuerpoS = physics->createRigidStatic(PxTransform(posPx, rotacion));
            shape = physics->createShape(PxBoxGeometry(tamPx / 2.0f), *mat, true);
            cuerpoS->attachShape(*shape);
            scene->addActor(*cuerpoS);

            // Asignamos userData para callbacks
            cuerpoS->userData = this;

            render = new RenderItem(shape, cuerpoS, color);
        }
    }

    // Constructor para esfera
    Solido(PxPhysics* physics, PxScene* scene, PxMaterial* mat,
        const Vector3D& pos, float radio, const PxVec4& color,
        Tipo tipo = Tipo::Dinamico,
        TipoEspecial te = TipoEspecial::Ninguno) // NUEVO
        : scene(scene), tipo(tipo), tipoEspecial(te)
    {
        PxVec3 posPx(pos.getX(), pos.getY(), pos.getZ());

        PxShape* shape = nullptr;

        if (tipo == Tipo::Dinamico) {
            cuerpoD = physics->createRigidDynamic(PxTransform(posPx));
            shape = physics->createShape(PxSphereGeometry(radio), *mat, true);
            cuerpoD->attachShape(*shape);
            PxRigidBodyExt::updateMassAndInertia(*cuerpoD, 1.0f);
            scene->addActor(*cuerpoD);

            // userData
            cuerpoD->userData = this;

            render = new RenderItem(shape, cuerpoD, color);
        }
        else {
            cuerpoS = physics->createRigidStatic(PxTransform(posPx));
            shape = physics->createShape(PxSphereGeometry(radio), *mat, true);
            cuerpoS->attachShape(*shape);
            scene->addActor(*cuerpoS);

            // userData
            cuerpoS->userData = this;

            render = new RenderItem(shape, cuerpoS, color);
        }
    }

    virtual ~Solido() {
        if (render) DeregisterRenderItem(render);
        if (cuerpoD) cuerpoD->release();
        if (cuerpoS) cuerpoS->release();
    }

    // Update solo para dinámicos
    virtual void update(float deltaTime) {
    }

    PxRigidDynamic* getCuerpoDinamico() { return cuerpoD; }
    PxRigidStatic* getCuerpoEstatico() { return cuerpoS; }
    Tipo getTipo() const { return tipo; }
    TipoEspecial getTipoEspecial() const { return tipoEspecial; } // NUEVO

protected:
    PxRigidDynamic* cuerpoD = nullptr;
    PxRigidStatic* cuerpoS = nullptr;
    RenderItem* render = nullptr;
    PxScene* scene = nullptr;
    Tipo tipo;
    TipoEspecial tipoEspecial; // NUEVO
};
