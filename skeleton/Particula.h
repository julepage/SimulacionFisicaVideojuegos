#pragma once
#include "Vector3D.h"
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"
#include <unordered_set>
#include <string>

class Particula
{
public:
	static physx::PxSphereGeometry defaultSphere;//geometría base
	//enum class TipoProyectil { ESFERA, CAJA, CAPSULA };//otra opcion chula pero d emomento nose si me encanta

	Particula(Vector3D pos, Vector3D vel, Vector3D acc, Vector4 color = Vector4(1.0f, 1.0f, 0.0f, 1.0f), physx::PxGeometry* forma = nullptr, float d = 1.0, float masa = 1.0, float g = 9.8, float vidas = 6.0f);
	virtual ~Particula() { DeregisterRenderItem(renderItem); renderItem = nullptr; };

	//constructora para fuegos artificiales
	Particula(Vector3D pos, Vector3D vel, Vector3D acc, bool yaExplotada, Vector4 color = Vector4(1.0f, 1.0f, 0.0f, 1.0f),
		physx::PxGeometry* forma = nullptr, float d = 1.0, float masa = 1.0, float g = 9.8, float vidas = 6.0f)
		: Particula(pos, vel, acc, color, forma, d, masa, g, vidas) // llama a la constructora principal
	{
		exploto = yaExplotada;
	}

	virtual void integrate(double t);

	Vector3D getVel() { return velocidad; };
	Vector3D getPos() { return Vector3D(pose.p.x, pose.p.y, pose.p.z); };
	//getter
	float getVidas() const noexcept { return vida; } ;
	float getMasa() const noexcept { return masa; };
	Vector3D getPosIni() const noexcept { return posIni; };
	Vector3D getPos()  const noexcept { return { pose.p.x, pose.p.y, pose.p.z }; };
	bool getHayFuerza() const noexcept { return hayFuerza; };
	//setter
	void setVidas(float v) noexcept { vida = v; };
	void setAc(Vector3D Acc) noexcept { ac = Acc; };//da una aceleracion
	void setHayFuerza(bool f) noexcept { hayFuerza = f; };
	void addFuerza(const Vector3D& f) { fuerzaAcumulada = fuerzaAcumulada + f; };//añade fuerza luego se suma en integrate


	//CONTROL DE FUERZAS PERMITIDAS!!!
	bool permiteFuerza(const std::string& tipo) const noexcept {
		return fuerzasPermitidas.empty() || fuerzasPermitidas.count(tipo) > 0;
	}

	void permitirFuerza(const std::string& tipo) noexcept {
		fuerzasPermitidas.insert(tipo);
	}

	void quitarFuerza(const std::string& tipo) noexcept {
		fuerzasPermitidas.erase(tipo);
	}


	bool exploto;
protected:
	Vector3D velocidad;
	physx::PxTransform pose;
	RenderItem* renderItem;
	Vector3D ac;
	Vector3D posIni;
	float damping;
	float masa;
	float gravedad;
	float vida;//Segumdo de vida
	Vector3D fuerzaAcumulada;
	//Estetico
	physx::PxGeometry* forma;
	Vector4 color;
	bool hayFuerza = true;//directamente desactiva todas las fierzas sobre la particula

	std::unordered_set<std::string> fuerzasPermitidas;//permite q le afecten ciertas fuerzas
};

