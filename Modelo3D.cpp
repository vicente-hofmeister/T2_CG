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

Face *faces;
unsigned int nFaces;

bool Modelo3D::LeObjeto(char *Nome) {
    ifstream arq;
    arq.open(Nome, ios::in);
    if (!arq)
    {
        cout << "Erro na abertura do arquivo " << Nome << "." << endl;
        return false;
    }
    arq >> nFaces;
    faces = new Face[nFaces];
    float x,y,z;
    for (int i=0;i<nFaces;i++)
    {
        // Le os tres vertices
        arq >> x >> y >> z; // Vertice 1
        faces[i].setP1(Ponto(x, y, z));
        arq >> x >> y >> z; // Vertice 2
        faces[i].setP2(Ponto(x, y, z));
        arq >> x >> y >> z; // Vertice 3
        faces[i].setP3(Ponto(x, y, z));
        cout << i << ": ";
    }
}

void Modelo3D::desenhar() const {
    glBegin(GL_TRIANGLES);
    for (unsigned int i = 0; i < nFaces; i++) {
        // Obtenha os vértices da face atual
        Ponto v1 = faces[i].getP1();
        Ponto v2 = faces[i].getP2();
        Ponto v3 = faces[i].getP3();

        // Desenhe o triângulo
        glVertex3f(v1.x, v1.y, v1.z);
        glVertex3f(v2.x, v2.y, v2.z);
        glVertex3f(v3.x, v3.y, v3.z);
    }
    glEnd();
}