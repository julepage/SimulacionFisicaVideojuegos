#include "FuenteFuego.h"
#include <iostream>

void FuenteFuego::emitir(float t)
{
	int numParticulas = static_cast<int>(tasaEmision * t);//cuantas salen
	if (numParticulas <= 0) numParticulas = 1;//por si acaso

	for (int i = 0; i < numParticulas; i++) {//las recorro
		//posiciones en la linea
		Vector3D posLinea = pos;
		posLinea.setX(posLinea.getX() + distLinea(generador));

		//que suba como las llamas
		Vector3D velParticula = dir * vel;
		velParticula.setX(velParticula.getX() + distLinea(generador) * 0.1f);
		velParticula.setZ(velParticula.getZ() + distLinea(generador) * 0.1f);

		//para que sean colores de fuego
		float tono = static_cast<float>(rand()) / RAND_MAX;
		Vector4 color;

		if (tono < 0.5f) color = Vector4(0.8f, 0.15f, 0.05f, 1.0f);//rojjjo
		else if (tono < 0.8f) color = Vector4(0.9f, 0.45f, 0.1f, 1.0f);//naranja
		else color = Vector4(0.95f, 0.7f, 0.3f, 1.0f);//amarillo

		//lo calculo según los datos de mi particula modelo
		Vector3D posi = modelo->getPos() + posLinea;
		Vector3D veloc = modelo->getVel() + velParticula;

		//creo las particulas
		Particula* p = new Particula(posi, veloc, modelo->getAc(), color, nullptr, 0.95f, 0.5f, 9.8f, 2.0f);

		p->setHayFuerza(false);//no le afecta ninguna fuerza
		particulas.push_back(p);
	}

}

void FuenteFuego::actualizar(float t)
{
	FuenteParticulas::actualizar(t);
	std::vector<Particula*> nuevas;
	for (auto p : particulas) {
		//si cae mas de mi limite se elimina
		if (p->getPos().getY() < pos.getY())
			delete p;
		else
			nuevas.push_back(p);
	}
	particulas = nuevas;
}

bool FuenteFuego::colisionConPelota() const
{
	//Vector3D p = pelota->getPos();
	//float rPelota = 1.0f; 

	//float yBase = pos.getY() - rPelota;
	//float yTope = pos.getY() + alturaFuego + rPelota;

	//if (p.getY() < yBase || p.getY() > yTope)
	//	return false;

	//Vector3D diff(p.getX() - pos.getX(), 0.0f, p.getZ() - pos.getZ());
	//return diff.modulo() <= radioFuego + rPelota; //  si la pelota ebtra en el radio del fuego se reposiciona

	Vector3D p = pelota->getPos();
	float rPelota = 1.0f;

	// altura (Y)
	float yBase = pos.getY() - rPelota;
	float yTope = pos.getY() + alturaFuego + rPelota;

	if (p.getY() < yBase || p.getY() > yTope)
		return false;

	// Diferencias en X y Z
	float dx = std::abs(p.getX() - pos.getX());
	float dz = std::abs(p.getZ() - pos.getZ());

	float radioX = radioFuego + rPelota;
	float radioZ = radioFuego * 0.25f + rPelota; // ? “un cuarto”

	return dx <= radioX && dz <= radioZ;
}