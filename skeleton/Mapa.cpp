#include "Mapa.h"
#include <fstream>
#include <iostream>

Mapa::Mapa(const std::string& filename,
    SistemaSolidos* sistema,
    PxPhysics* physics,
    PxScene* scene,
    PxMaterial* mat)
    : sistemaSolidos(sistema), gPhysics(physics), gScene(scene), gMaterial(mat)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "No se pudo abrir el archivo del mapa: " << filename << std::endl;
        return;
    }

    int width, height;
    file >> width >> height;
    std::string line;
    std::getline(file, line); // consumir salto de línea

    Vector3D startPos(0, 0, 0);

    // Primero leemos todas las líneas del mapa
    std::vector<std::string> mapaLines;
    for (int y = 0; y < height; ++y) {
        std::getline(file, line);
        mapaLines.push_back(line);
    }

    for (int y = 0; y < height; ++y) {
        const std::string& currentLine = mapaLines[y];
        for (int x = 0; x < (int)currentLine.size(); ++x) {
            char c = currentLine[x];

            Vector3D pos;
            Vector3D tam;
            PxVec4 color;

            if (c == 's') {
                // Suelo
                tam = Vector3D(20.0f, 2.0f, 20.0f);
                pos = startPos + Vector3D(x * tam.getX(), tam.getY() / 2.0f, y * tam.getZ());
                color = PxVec4(0.0f, 0.5f, 0.0f, 1.0f);
            }
            else if (c == 'x') {
                // Bordes: pegados al suelo, misma profundidad que el suelo
                tam = Vector3D(20.0f, 5.0f, 20.0f);
                pos = startPos + Vector3D(x * tam.getX(), 2.0f + tam.getY() / 2.0f, y * tam.getZ()); // encima del suelo
                color = PxVec4(0.0f, 0.5f, 0.0f, 1.0f);
            }
            else if (c == 'm') {
                // Meta
                tam = Vector3D(20.0f, 2.0f, 20.0f);
                pos = startPos + Vector3D(x * tam.getX(), tam.getY() / 2.0f, y * tam.getZ());
                color = PxVec4(1.0f, 0.8f, 0.0f, 1.0f);
                Solido* bloque = new Solido(gPhysics, gScene, gMaterial, pos, tam, color, Solido::Tipo::Estatico);
                sistemaSolidos->addSolido(bloque);
                pos = startPos + Vector3D(x * tam.getX(), tam.getY() / 2.0f, y * tam.getZ());
                tam = Vector3D(5.0f, 3.0f, 5.0f);
                color = PxVec4(0.0f, 0.5f, 0.0f, 1.0f);
            }
            else {
                continue;
            }

            Solido* bloque = new Solido(gPhysics, gScene, gMaterial, pos, tam, color, Solido::Tipo::Estatico);
            sistemaSolidos->addSolido(bloque);
        }
    }

    file.close();
}
