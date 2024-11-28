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


bool Modelo3D::LeObjetoCompleto(const char *Nome)
{
    ifstream arq;
    arq.open(Nome, ios::in);
    if (!arq)
    {
        cout << "Erro na abertura do arquivo " << Nome << "." << endl;
        return false;
    }
    int nGrupos, nText;
    char tipo;
    arq >> tipo;
    arq >> nFaces;
    arq >> nGrupos >> nText;

    faces = new Face[nFaces];

    for (int i = 0; i < nFaces; i++)
    {
        float x, y, z;
        unsigned int cor;
        int grupo = -1;

        // Le os tres vertices
        arq >> x >> y >> z; // Vertice 1
        faces[i].setP1(Ponto(x, y, z));
        arq >> x >> y >> z; // Vertice 2
        faces[i].setP2(Ponto(x, y, z));
        arq >> x >> y >> z; // Vertice 3
        faces[i].setP3(Ponto(x, y, z));

        arq >> std::hex >> cor;
        faces[i].setCor(cor);

        if (!(arq >> grupo))
        {                // Tenta ler o grupo, se não existir mantém o padrão
            arq.clear(); // Limpa o estado de erro
        }
        faces[i].setGrupo(grupo);
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

