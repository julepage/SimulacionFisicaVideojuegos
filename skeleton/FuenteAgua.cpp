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

		Particula* p = new Particula(pos, velParticula, Vector3D(0, -10, 0));
		p->setHayFuerza(false);//PARA QUE NO AFECTEN FUERZAS	
		particulas.push_back(p);
	}
}
