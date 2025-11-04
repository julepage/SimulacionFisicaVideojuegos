#pragma once
#include "Vector3D.h"
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"

class Particula
{
public:
	static physx::PxSphereGeometry defaultSphere;//geometría base
	//enum class TipoProyectil { ESFERA, CAJA, CAPSULA };//otra opcion chula pero d emomento nose si me encanta

	Particula(Vector3D pos, Vector3D vel, Vector3D acc, Vector4 color = Vector4(1.0f, 1.0f, 0.0f, 1.0f), physx::PxGeometry* forma = nullptr, float d = 1.0, float masa = 1.0, float g = 9.8, float vidas = 6.0f);
	virtual ~Particula() { DeregisterRenderItem(renderItem); renderItem = nullptr; };

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
	void setVidas(float v) noexcept { vida = v; }
	void setAc(Vector3D Acc) noexcept { ac = Acc; }
	void setHayFuerza(bool f) noexcept { hayFuerza = f; };

	void addFuerza(const Vector3D& f) { fuerzaAcumulada = fuerzaAcumulada + f; };

	//constructora para fuegos artificiales
	Particula(Vector3D pos, Vector3D vel, Vector3D acc, bool yaExplotada, Vector4 color = Vector4(1.0f, 1.0f, 0.0f, 1.0f),
		physx::PxGeometry* forma = nullptr, float d = 1.0, float masa = 1.0, float g = 9.8, float vidas = 6.0f)
		: Particula(pos, vel, acc, color, forma, d, masa, g, vidas) // llama a la constructora principal
	{
		exploto = yaExplotada;
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
	bool hayFuerza = true;
};

