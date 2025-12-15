#pragma once
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"
#include "Vector3D.h"

using namespace physx;

class Solido {
public:
    enum class Tipo { Estatico, Dinamico };

    // Constructor para cubo
    Solido(PxPhysics* physics, PxScene* scene, PxMaterial* mat,
        const Vector3D& pos, const Vector3D& tam, const PxVec4& color,
        Tipo tipo = Tipo::Dinamico)
        : scene(scene), tipo(tipo)
    {
        PxVec3 posPx(pos.getX(), pos.getY(), pos.getZ());
        PxVec3 tamPx(tam.getX(), tam.getY(), tam.getZ());

        PxShape* shape = nullptr;

        if (tipo == Tipo::Dinamico) {
            cuerpoD = physics->createRigidDynamic(PxTransform(posPx));
            shape = physics->createShape(PxBoxGeometry(tamPx / 2.0f), *mat, true);
            cuerpoD->attachShape(*shape);
            PxRigidBodyExt::updateMassAndInertia(*cuerpoD, 1.0f);
            scene->addActor(*cuerpoD);

            render = new RenderItem(shape, cuerpoD, color);
        }
        else {
            cuerpoS = physics->createRigidStatic(PxTransform(posPx));
            shape = physics->createShape(PxBoxGeometry(tamPx / 2.0f), *mat, true);
            cuerpoS->attachShape(*shape);
            scene->addActor(*cuerpoS);

            render = new RenderItem(shape, cuerpoS, color);
        }
    }

    // Constructor para esfera
    Solido(PxPhysics* physics, PxScene* scene, PxMaterial* mat,
        const Vector3D& pos, float radio, const PxVec4& color,
        Tipo tipo = Tipo::Dinamico)
        : scene(scene), tipo(tipo)
    {
        PxVec3 posPx(pos.getX(), pos.getY(), pos.getZ());

        PxShape* shape = nullptr;

        if (tipo == Tipo::Dinamico) {
            cuerpoD = physics->createRigidDynamic(PxTransform(posPx));
            shape = physics->createShape(PxSphereGeometry(radio), *mat, true);
            cuerpoD->attachShape(*shape);
            PxRigidBodyExt::updateMassAndInertia(*cuerpoD, 1.0f);
            scene->addActor(*cuerpoD);

            render = new RenderItem(shape, cuerpoD, color);
        }
        else {
            cuerpoS = physics->createRigidStatic(PxTransform(posPx));
            shape = physics->createShape(PxSphereGeometry(radio), *mat, true);
            cuerpoS->attachShape(*shape);
            scene->addActor(*cuerpoS);

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
        if (!cuerpoD) return;

        PxVec3 vel = cuerpoD->getLinearVelocity();
        vel.x *= 0.95f; // damping X
        vel.z *= 0.95f; // damping Z
        cuerpoD->setLinearVelocity(vel);
    }

    PxRigidDynamic* getCuerpoDinamico() { return cuerpoD; }
    PxRigidStatic* getCuerpoEstatico() { return cuerpoS; }
    Tipo getTipo() const { return tipo; }

protected:
    PxRigidDynamic* cuerpoD = nullptr;
    PxRigidStatic* cuerpoS = nullptr;
    RenderItem* render = nullptr;
    PxScene* scene = nullptr;
    Tipo tipo;
};
