#include "Mapa.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "PxPhysicsAPI.h"

Mapa::Mapa(const std::string& filename,
	SistemaSolidos* sistema,
	PxPhysics* physics,
	PxScene* scene,
	PxMaterial* mat)
	: sistemaSolidos(sistema), gPhysics(physics), gScene(scene), gMaterial(mat)
{
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "No se pudo abrir el archivo del mapa\n";
		return;
	}

	float iniX, iniY, iniZ;
	file >> iniX >> iniY >> iniZ;
	file.ignore(1000, '\n'); // ignorar resto de línea

	Vector3D startPos(iniX, iniY, iniZ);

	float tamX = 20.f, tamY = 2.f, tamZ = 20.f;
	float currentY = iniY;
	PxQuat currentRot(PxIdentity);

	float mapOffsetX = 0.f; // Posición X global del mapa
	float mapOffsetZ = 0.f; // <-- nueva variable global Z

	std::vector<std::string> sectionLines;

	// Última posición X ocupada
	float lastSectionWidth = 0.f;


	auto dibujarSeccion = [&]() {
		if (sectionLines.empty()) return;

		int filas = sectionLines.size();
		int columnas = 0;
		for (auto& l : sectionLines) if ((int)l.size() > columnas) columnas = l.size();

		// Dibujar bloques
		for (int z = 0; z < filas; ++z) {
			int lastBlockInFila = -1;
			for (int x = 0; x < (int)sectionLines[z].size(); ++x) {
				char c = sectionLines[z][x];

				// Solo dibujamos si hay bloque real
				if (c == 's' || c == 'x' || c == 'm') {
					Vector3D tam(tamX, tamY, tamZ);
					PxVec4 color(0.f, 0.5f, 0.f, 1.f);

					if (c == 'x') tam = Vector3D(tamX, 5.f, tamZ);
					if (c == 'm') color = PxVec4(1.f, 0.8f, 0.f, 1.f);

					Vector3D pos(
						startPos.getX() + mapOffsetX + x * tamX,
						currentY + tam.getY() / 2.f,
						startPos.getZ() + mapOffsetZ + z * tamZ
					);
					Solido* bloque;
					if (c == 'm') {
					 bloque = new Solido(
							gPhysics, gScene, gMaterial,
							pos, tam, color,
							Solido::Tipo::Estatico,
							currentRot, Solido::TipoEspecial::Meta
						);

					}
					else
					{
						bloque = new Solido(
							gPhysics, gScene, gMaterial,
							pos, tam, color,
							Solido::Tipo::Estatico,
							currentRot
						);
					}
					sistemaSolidos->addSolido(bloque);

					lastBlockInFila = x; // actualizar último bloque real
				}
			}

			if (lastBlockInFila >= 0)
				lastSectionWidth = PxMax(lastSectionWidth, (lastBlockInFila + 1) * tamX);
		}


		// Actualizar offset X para la siguiente sección
		lastSectionWidth = columnas * tamX;
		mapOffsetX += lastSectionWidth;
		mapOffsetZ = tamZ; // <-- acumular Z
		sectionLines.clear();
		};

	std::string line;
	while (std::getline(file, line)) {
		while (!line.empty() && line.back() == '\r') line.pop_back();
		if (line.empty()) continue;

		// Rampa inclinada
		if (line[0] == 'I') {
			dibujarSeccion(); // dibuja la sección anterior antes de la rampa

			float angGrados = std::stof(line.substr(1));
			float angRad = angGrados * PxPi / 180.f;

			float yInicio = currentY - tamY / 2.18;
			float yFin = iniY; // altura final
			if (angGrados < 0) std::swap(yInicio, yFin);

			// Leer la forma de la rampa desde el txt (ej: "xsx")
			std::vector<std::string> formaRampa;
			while (std::getline(file, line)) {
				while (!line.empty() && line.back() == '\r') line.pop_back();
				if (line.empty()) break;
				if (isdigit(line[0]) || line[0] == '-' || line[0] == 'I') {
					file.seekg(-static_cast<int>(line.size()) - 1, std::ios_base::cur);
					break;
				}
				formaRampa.push_back(line);
			}

			int filas = formaRampa.size();
			int columnas = formaRampa.empty() ? 1 : formaRampa[0].size();

			float offsetXretroceso = tamX / 1.7; // cuánto retrocede la rampa al inicio
			float longitudX = fabs(yFin - yInicio) / sin(angRad) + offsetXretroceso; // longitud total
			float centerX = mapOffsetX + longitudX / 1.98f - offsetXretroceso; // centro ajustado

			float totalZ = columnas * tamZ; // ancho total según la forma

			float lastS_Z = 0.f;
			for (int z = 0; z < filas; ++z) {
				for (int x = 0; x < columnas; ++x) {
					if (formaRampa[z][x] == 's' || formaRampa[z][x] == 'm') {
						lastS_Z = z * tamZ;
					}
				}
			}

			// Posición central del bloque
			Vector3D pos(
				startPos.getX() + centerX,
				(yInicio + yFin) / 2.f + tamY / 2.2f,
				startPos.getZ() + lastS_Z + mapOffsetZ
			);

			Vector3D tam(longitudX, tamY, totalZ);

			PxQuat rot(-angRad, PxVec3(0, 0, 1));

			// Crear rampa como un único bloque
			Solido* rampa = new Solido(
				gPhysics, gScene, gMaterial,
				pos, tam, PxVec4(0.f, 0.5f, 0.f, 1.f),
				Solido::Tipo::Estatico,
				rot
			);
			sistemaSolidos->addSolido(rampa);

			// ---- Crear los bordes a los lados ----
			float bordeTamZ = tamZ; // ancho de un bloque normal
			float bordeTamY = 5.0f; // altura del borde
			PxVec4 colorBorde(0.f, 0.5f, 0.f, 1.f); // verde oscuro
			for (int lado = -1; lado <= 1; lado += 2) { // izquierda y derecha
				Vector3D posBorde(
					pos.getX() - bordeTamY / 17,
					pos.getY() + bordeTamY / 3.25,
					pos.getZ() + lado * (totalZ / 2.f + bordeTamZ / 2.f) // afuera de la rampa
				);

				Vector3D tamBorde(longitudX, bordeTamY, bordeTamZ);

				Solido* borde = new Solido(
					gPhysics, gScene, gMaterial,
					posBorde, tamBorde, colorBorde,
					Solido::Tipo::Estatico,
					rot
				);
				sistemaSolidos->addSolido(borde);
			}

			// Actualizar offset para la siguiente sección
			mapOffsetX += longitudX - offsetXretroceso;
			currentY = yFin;
			currentRot = PxQuat(PxIdentity);
			mapOffsetZ = 0;
			continue;
		}


		// Cambio de altura
		if (isdigit(line[0]) || line[0] == '-') {
			dibujarSeccion();
			currentY += std::stof(line);
			continue;
		}

		// Línea normal dentro de sección
		sectionLines.push_back(line);
	}

	// Dibujar la última sección
	dibujarSeccion();
}
