#include "Flotacion.h"

void Flotacion::updateFuerzas(Particula* p, double t)
{
	if (!activa) return;
	if (!p->permiteFuerza("flotacion")) return;

	float h = p->getPos().getY();
	float h0 = liquidoH;//donde pongo mi plano que simula agua

	float sumergido;
	Vector3D f(0, 0, 0);
	if (h - h0 > H / 2) sumergido = 0.0;
	else if (h0 - h > H / 2) sumergido = 1.0;
	else sumergido = (h0 - h) / H + 0.5;

	f.setY(d * V * sumergido * g);

	p->addFuerza(f);
}

void Flotacion::toggle()
{
	GeneradorFuerzas::toggle();
	if (liquido != nullptr) {
		DeregisterRenderItem(liquido);
		liquido = nullptr;
	}
	else {
		liquido = new RenderItem(CreateShape(PxBoxGeometry(30.0f, 0.10f, 30.0f)),
			new PxTransform(PxVec3(pos.getX(), pos.getY(), pos.getZ())), Vector4(0.0f, 0.0f, 1.0f, 1.0f));
	}
}
