#include "FuenteAgua.h"

void FuenteAgua::emitir(float t)
{
	//particulas a emitir
	int numParticulas = static_cast<int> (tasaEmision * t);//uso static cast para pasar de float a int porque uso todo floats

	if (numParticulas == 0) numParticulas = 5;
	//creacion de particulas
	for (int i = 0; i < numParticulas;i++) {

		Vector3D velParticula = dir * vel;

		velParticula.setX(velParticula.getX() + distUniforme(generador));
		velParticula.setY(velParticula.getY() + distUniforme(generador));
		velParticula.setZ(velParticula.getZ() + distUniforme(generador));

		Vector3D posi = modelo->getPos() + pos;
		Vector3D veloc = modelo->getVel() + velParticula;
		
		Particula* p = new Particula(posi, veloc, modelo->getAc(), modelo->getColor());
		p->permitirFuerza("tunelViento");
		p->permitirFuerza("explosion");
		p->permitirFuerza("torbellino");
		particulas.push_back(p);
	}
}

//PARA EL TUNEL DE VIENTO
//int numParticulas = static_cast<int>(tasaEmision * t);
//if (numParticulas == 0) numParticulas = 5;
//
//float radioCircunferencia = 15.0f; // radio del grosor del chorro
//std::uniform_real_distribution<float> distAng(0.0f, 2.0f * 3.14159265f); // ángulo circunferencia
//std::uniform_real_distribution<float> distRadio(0.0f, radioCircunferencia);  // radio aleatorio dentro del círculo
//
//for (int i = 0; i < numParticulas; i++) {
//    // Velocidad del chorro
//    Vector3D velParticula = dir * vel;
//    velParticula.setX(velParticula.getX() + distUniforme(generador));
//    velParticula.setY(velParticula.getY() + distUniforme(generador));
//    velParticula.setZ(velParticula.getZ() + distUniforme(generador));
//
//    // Generar posición inicial dentro de la circunferencia perpendicular a dir
//    float ang = distAng(generador);
//    float r = distRadio(generador);
//
//    // Vectores perpendiculares a dir
//    Vector3D dirNorm = dir.normalize();
//    Vector3D up(0, 1, 0);
//    if (fabs(dirNorm.productoEscalar(up)) > 0.99f) up = Vector3D(1, 0, 0); // evitar paralelismo
//    Vector3D right = dirNorm.cross(up).normalize();
//    Vector3D forward = right.cross(dirNorm).normalize();
//
//    // Offset en la circunferencia
//    Vector3D offset = right * cos(ang) * r + forward * sin(ang) * r;
//
//    // Posición inicial
//    Vector3D posi = modelo->getPos() + pos + offset;
//
//    // Velocidad total
//    Vector3D veloc = modelo->getVel() + velParticula;
//
//    // Crear partícula
//    Particula* p = new Particula(posi, veloc, modelo->getAc(), modelo->getColor());
//    p->permitirFuerza("tunelViento");
//    p->permitirFuerza("explosion");
//    p->permitirFuerza("torbellino");
//
//    particulas.push_back(p);
//}