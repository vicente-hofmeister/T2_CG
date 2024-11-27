#include "Modelo3D.h"
#include <fstream>
#include <iostream>

#ifdef WIN32
#include <windows.h>
#include <glut.h>
#else
#include <sys/time.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <GL/glut.h>
#endif

bool Modelo3D::carregarModelo(const std::string& caminhoArquivo) {
    std::ifstream arquivo(caminhoArquivo);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << caminhoArquivo << std::endl;
        return false;
    }

    float x, y, z;
    while (arquivo >> x >> y >> z) {
        vertices.emplace_back(x, y, z);
    }
    arquivo.close();
    std::cout << "Modelo carregado com sucesso: " << caminhoArquivo << std::endl;
    return true;
}

void Modelo3D::desenhar() const {
    glBegin(GL_TRIANGLES);
    for (const auto& vertice : vertices) {
        glVertex3f(vertice.x, vertice.y, vertice.z);
    }
    glEnd();
}

const std::vector<Ponto>& Modelo3D::getVertices() const {
    return vertices;
}
