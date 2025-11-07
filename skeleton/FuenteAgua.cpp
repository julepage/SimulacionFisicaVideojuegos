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
