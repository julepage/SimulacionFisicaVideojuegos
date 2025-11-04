#include "FuenteFuegosArtificiales.h"

void FuenteFuegosArtificiales::emitir(float t)
{
	//para no saturar que explota el programa sino
	if (particulas.size() < 5) {
		Vector3D velP = dir * vel;//velocidad de laparticula
		//uso distr uniforme, en y no pq quiero que vaya hacia arriba
		velP.setX(velP.getX() + distUniforme(generador) * 0.1f);
		velP.setZ(velP.getZ() + distUniforme(generador) * 0.1f);

		//crea el cohete qu esubira
		Particula* p = new Particula(pos, velP, Vector3D(0, -10, 0));
		p->exploto = false;
		particulas.push_back(p);
	}
}

void FuenteFuegosArtificiales::actualizar(float t)
{
	//particulas que explotan y salne mas:
	std::vector<Particula*> nuevas;

	for (auto p : particulas) {
		p->integrate(t);

		if (p->getVel().getY() < 0 && !p->exploto) {
			p->exploto = true;//si la particula cae y aun no ha explotado pues la exploto y la marco
			//explosion
			for (int i = 0; i < 10; ++i) {
				//subparticulas de la explosion
				Vector3D velSub(distUniforme(generador), distUniforme(generador) + 0.5, distUniforme(generador));//mas arriba en y
				velSub = velSub.normalize() * 5.0f;//si explota mas o menos, es su vel
				//COLORES ALEATORIOS!!!
				Vector4 color(distColor(generador), distColor(generador), distColor(generador), 1.0f);
				
				//salen en la misma pos que la particula de la que salen, con otra vel color...
				Particula* sub = new Particula(p->getPos(), velSub, Vector3D(0, -10, 0), color, nullptr, 0.98f, 0.5f, 9.8f, 2.0f);
				nuevas.push_back(sub);
			}
			delete p;//quiero quitar el cohete
		}
		else nuevas.push_back(p);
	}

	particulas = nuevas;//reemplazo el vector para incluir las nuevas subpart
}
