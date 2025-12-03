#pragma once
#include "GeneradorFuerzas.h"
using namespace physx;

class Flotacion: public GeneradorFuerzas
{
public:
	Flotacion(Vector3D pos, float h, float V, float d)
		: pos(pos), V(V), d(d), H(h)  {
		activa = true;
		liquido = new RenderItem(CreateShape(PxBoxGeometry(30.0f, 0.10f, 30.0f)),
			new PxTransform(PxVec3(pos.getX(), pos.getY(), pos.getZ())), Vector4(0.0f, 0.0f, 1.0f, 1.0f));
		liquidoH = pos.getY();
	}
	~Flotacion() { if (liquido != nullptr) { DeregisterRenderItem(liquido);  liquido = nullptr; } };

	void updateFuerzas(Particula* p, double t) override;
	void toggle() override;

protected:
	Vector3D pos;
	float  liquidoH, H, V, d;
	const float g = 9.8;
	RenderItem* liquido;
};


