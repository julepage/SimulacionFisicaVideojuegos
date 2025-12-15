#pragma once
#include <vector>
#include <algorithm>
#include "Solido.h"

class SistemaSolidos {
public:
    ~SistemaSolidos() {
        for (auto s : solidos) delete s;
        solidos.clear();
    }

    void addSolido(Solido* s) {
        if (s) solidos.push_back(s);
    }

    void eliminarSolido(Solido* s) {
        auto it = std::find(solidos.begin(), solidos.end(), s);
        if (it != solidos.end()) {
            delete* it;
            solidos.erase(it);
        }
    }

    void actualizar(float deltaTime) {
        for (auto s : solidos) {
            if (!s) continue;
            if (s->getTipo() == Solido::Tipo::Dinamico) {
                s->update(deltaTime);
            }
        }
    }

    // Opción para limpiar todos los sólidos
    void limpiar() {
        for (auto s : solidos) delete s;
        solidos.clear();
    }

private:
    std::vector<Solido*> solidos;
};
