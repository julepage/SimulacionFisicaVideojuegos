#include "FuenteFuegosArtificiales.h"

void FuenteFuegosArtificiales::emitir(float t)
{
	//para no saturar que explota el programa sino
	if (particulas.size() < 5) {
		Vector3D velP = dir * vel;//velocidad de laparticula
		//uso distr uniforme, en y no pq quiero que vaya hacia arriba
		velP.setX(velP.getX() + distUniforme(generador) * 0.1f);
		velP.setZ(velP.getZ() + distUniforme(generador) * 0.1f);

		Vector3D posi = modelo->getPos() + pos;
		Vector3D veloc = modelo->getVel() + velP;

		//crea el cohete qu esubira
		Particula* p = new Particula(posi, veloc, modelo->getAc(), modelo->getColor());
		p->exploto = false;
		//p->permitirFuerza("viento");
        p->setHayFuerza(false);
		particulas.push_back(p);
	}
}

void FuenteFuegosArtificiales::actualizar(float t)
{
    //a estas si les permito todas las fuerzas
    std::vector<Particula*> nuevas;
    FuenteParticulas::actualizar(t);

    for (auto p : particulas) {
        //si está cayendo y no ha explotado pues explota
        if (p->getVel().getY() < 0 && !p->exploto) {
            p->exploto = true; //explotada para q no lo haga infiinico

            const int numSubParticulas = 50;
            const float velocidadExplosion = 15.0f;

            for (int i = 0; i < numSubParticulas; ++i) {
                //explosion con vals aleatorios
                Vector3D velSub( distUniforme(generador) * 2.0f - 1.0f, distUniforme(generador) * 2.0f - 1.0f, distUniforme(generador) * 2.0f - 1.0f);
                velSub = velSub.normalize() * velocidadExplosion;

                //color aleatorio
                Vector4 color(distColor(generador), distColor(generador), distColor(generador), 1.0f);

                //pos ini particula
                Vector3D posi = modelo->getPos() + p->getPos();
                Vector3D veloc = modelo->getVel() + velSub;

                //creo la subparticula
                Particula* sub = new Particula(posi, veloc, modelo->getAc(), color,
                    nullptr, 0.98f, 0.5f, 9.8f, 5.0f);
                nuevas.push_back(sub);
            }

            delete p;//elimino la particula original
        }
        else {
            nuevas.push_back(p);
        }
    }
    particulas = nuevas;
}
