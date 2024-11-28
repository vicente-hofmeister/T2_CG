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

#include "Ponto.h"
#include "Face.h"
#include "Poliedro.h"
#include <vector>
#include <sstream>

Face *faces;
unsigned int nFaces;
Poliedro boundingBox;

bool Modelo3D::LeObjetoSimples(char *Nome)
{
    ifstream arq;
    arq.open(Nome, ios::in);
    if (!arq)
    {
        cout << "Erro na abertura do arquivo " << Nome << "." << endl;
        return false;
    }

    arq >> nFaces;
    if (nFaces <= 0)
    {
        cout << "Erro: Número inválido de faces no arquivo." << endl;
        return false;
    }

    faces = new Face[nFaces]; // Aloca memória para as faces

    float x, y, z;
    for (int i = 0; i < nFaces; i++)
    {
        // Lê os três vértices
        arq >> x >> y >> z;
        Ponto p1(x, y, z);

        arq >> x >> y >> z;
        Ponto p3(x, y, z);

        arq >> x >> y >> z;
        Ponto p2(x, y, z);

        faces[i] = Face(p1, p2, p3); // Usa o construtor com 3 pontos
    }

    return true;
}

void Modelo3D::DesenharSimples() const
{
    glBegin(GL_TRIANGLES);
    for (unsigned int i = 0; i < nFaces; i++)
    {
        // Obtenha os vértices da face atual
        Ponto p1 = faces[i].getP1();
        Ponto p2 = faces[i].getP2();
        Ponto p3 = faces[i].getP3();

        Ponto normal, v1, v2;

        // Calcule os vetores
        v1 = p3 - p1;
        v2 = p2 - p1;

        // Calcule o produto vetorial (normal da face)
        ProdVetorial(v1, v2, normal);

        // Normalize a normal
        float magnitude = sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
        if (magnitude != 0) {
            normal.x /= magnitude;
            normal.y /= magnitude;
            normal.z /= magnitude;
        }

        // Defina a normal
        glNormal3f(normal.x, normal.y, normal.z);

        // Defina os vértices
        glVertex3f(p1.x, p1.y, p1.z);
        glVertex3f(p2.x, p2.y, p2.z);
        glVertex3f(p3.x, p3.y, p3.z);
    }
    glEnd();
}

Poliedro Modelo3D::CalcularBoundingBoxModelo()
{
    float MinX = std::numeric_limits<float>::max();
    float MinY = std::numeric_limits<float>::max();
    float MinZ = std::numeric_limits<float>::max();

    float MaxX = std::numeric_limits<float>::lowest();
    float MaxY = std::numeric_limits<float>::lowest();
    float MaxZ = std::numeric_limits<float>::lowest();

    boundingBox.setMin(Ponto(MinX, MinY, MinZ));
    boundingBox.setMax(Ponto(MaxX, MaxY, MaxZ));

    for (unsigned int i = 0; i < nFaces; i++)
    {
        Face &face = faces[i];

        Ponto p1 = face.getP1();
        Ponto p2 = face.getP2();
        Ponto p3 = face.getP3();

        // Poliedro boundingBoxFace = face.CalcularBoundingBox();

        Ponto minBBox = boundingBox.getMin();
        Ponto maxBBox = boundingBox.getMax();

        // Ponto minFace = boundingBoxFace.getMin();
        // Ponto maxFace = boundingBoxFace.getMax();

        boundingBox.setMin(Ponto(std::min(minBBox.x, std::min(p1.x, std::min(p2.x, p3.x))),
                                 std::min(minBBox.y, std::min(p1.y, std::min(p2.y, p3.y))),
                                 std::min(minBBox.z, std::min(p1.z, std::min(p2.z, p3.z)))));

        boundingBox.setMax(Ponto(std::max(maxBBox.x, std::max(p1.x, std::max(p2.x, p3.x))),
                                 std::max(maxBBox.y, std::max(p1.y, std::max(p2.y, p3.y))),
                                 std::max(maxBBox.z, std::max(p1.z, std::max(p2.z, p3.z)))));
    }

    return boundingBox;
}