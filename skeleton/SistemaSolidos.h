#pragma once
#include <vector>
#include <algorithm>
#include <iostream>
#include "Solido.h"
#include "globales.h"

class SistemaSolidos {
public:
	~SistemaSolidos() {
		for (auto s : solidos) delete s;
		solidos.clear();
	}

	void addSolido(Solido* s) {
		if (s) solidos.push_back(s);
		if (s->getTipoEspecial() == Solido::TipoEspecial::Meta)
			meta = s;
	}

	void eliminarSolido(Solido* s) {
		auto it = std::find(solidos.begin(), solidos.end(), s);
		if (it != solidos.end()) {
			delete* it;
			solidos.erase(it);
		}
	}

	void actualizar(float deltaTime) {
		for (auto s : solidos) {
			if (!s) continue;
			if (s->getTipo() == Solido::Tipo::Dinamico) {
				s->update(deltaTime);
			}
		}

		// Comprobar colisión con la meta
		if (meta) {
			if (chocan(pelota, meta) && !ganaste) { // 'pelota' es la variable global
				std::cout << "¡Ganaste! La pelota tocó la meta.\n";
				ganaste = true;
			}
		}
	}

	// Opción para limpiar todos 
	void limpiar() {
		for (auto s : solidos) delete s;
		solidos.clear();
	}

	bool chocan(Solido* pelota, Solido* meta) {
		// Posiciones
		PxVec3 pPos = pelota->getCuerpoDinamico()->getGlobalPose().p;
		PxVec3 mPos = meta->getCuerpoEstatico()->getGlobalPose().p;

		// Tamaños
		float radioPelota = 2.0f; // ajusta al radio de tu esfera
		Vector3D tamMeta(20, 2, 20); // tamaño real de tu bloque meta

		// Límites AABB de la pelota
		float pMinX = pPos.x - radioPelota, pMaxX = pPos.x + radioPelota;
		float pMinY = pPos.y - radioPelota, pMaxY = pPos.y + radioPelota;
		float pMinZ = pPos.z - radioPelota, pMaxZ = pPos.z + radioPelota;

		// Límites AABB de la meta
		float mMinX = mPos.x - tamMeta.getX() / 2.0f, mMaxX = mPos.x + tamMeta.getX() / 2.0f;
		float mMinY = mPos.y - tamMeta.getY() / 2.0f, mMaxY = mPos.y + tamMeta.getY() / 2.0f;
		float mMinZ = mPos.z - tamMeta.getZ() / 2.0f, mMaxZ = mPos.z + tamMeta.getZ() / 2.0f;

		// Comprobación AABB
		bool overlapX = (pMaxX >= mMinX) && (pMinX <= mMaxX);
		bool overlapY = (pMaxY >= mMinY) && (pMinY <= mMaxY);
		bool overlapZ = (pMaxZ >= mMinZ) && (pMinZ <= mMaxZ);

		return overlapX && overlapY && overlapZ;
	}

	PxRigidActor* getCuerpoActor(Solido* s) {
		if (s->getTipo() == Solido::Tipo::Dinamico)
			return s->getCuerpoDinamico();
		else
			return s->getCuerpoEstatico();
	}

	bool intersecan(Solido* a, Solido* b) {
		PxRigidActor* ra = getCuerpoActor(a);
		PxRigidActor* rb = getCuerpoActor(b);

		if (!ra || !rb) return false;

		return ra->getWorldBounds().intersects(rb->getWorldBounds());
	}

	bool victoria() { return ganaste; };
private:
	std::vector<Solido*> solidos;
	Solido* meta = nullptr;
	bool ganaste = false;
};
