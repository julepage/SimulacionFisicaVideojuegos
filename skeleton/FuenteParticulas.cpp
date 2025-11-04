#include "FuenteParticulas.h"

void FuenteParticulas::actualizar(float t)
{
    for (auto p : particulas)//recorro todas las particulas
    {
        p->integrate(t);//para moverlas
    }
}
