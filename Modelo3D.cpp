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

bool Modelo3D::LeObjetoSimples(char *Nome) {
    ifstream arq;
    arq.open(Nome, ios::in);
    if (!arq) {
        cout << "Erro na abertura do arquivo " << Nome << "." << endl;
        return false;
    }

    arq >> nFaces;
    if (nFaces <= 0) {
        cout << "Erro: Número inválido de faces no arquivo." << endl;
        return false;
    }

    faces = new Face[nFaces]; // Aloca memória para as faces

    float x, y, z;
    for (int i = 0; i < nFaces; i++) {
        // Lê os três vértices
        arq >> x >> y >> z; 
        Ponto p1(x, y, z);

        arq >> x >> y >> z;
        Ponto p2(x, y, z);

        arq >> x >> y >> z;
        Ponto p3(x, y, z);

        faces[i] = Face(p1, p2, p3); // Usa o construtor com 3 pontos
    }

    return true;
}


bool Modelo3D::LeObjetoCompleto(const char *Nome) {
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
    
    for (int i=0;i<nFaces;i++)
    {
        float x,y,z;
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

        if (!(arq >> grupo)) { // Tenta ler o grupo, se não existir mantém o padrão
            arq.clear();       // Limpa o estado de erro
        }
        faces[i].setGrupo(grupo);
    }
    return true;
}

void Modelo3D::DesenharSimples() const {
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

void Modelo3D::DesenharCompleto() const {
    glBegin(GL_TRIANGLES);
    for (unsigned int i = 0; i < nFaces; i++) {
        // Obtenha os vértices da face atual
        Ponto v1 = faces[i].getP1();
        Ponto v2 = faces[i].getP2();
        Ponto v3 = faces[i].getP3();

        unsigned int cor = faces[i].getCor();
        float r = ((cor >> 16) & 0xFF) / 255.0f;
        float g = ((cor >> 8) & 0xFF) / 255.0f;
        float b = (cor & 0xFF) / 255.0f;

        glColor3f(r, g, b);

        // Desenhe o triângulo
        glVertex3f(v1.x, v1.y, v1.z);
        glVertex3f(v2.x, v2.y, v2.z);
        glVertex3f(v3.x, v3.y, v3.z);
    }
    glEnd();
}


bool Modelo3D::LeObjetoOBJ(char *Nome) {
    ifstream arq;
    arq.open(Nome, ios::in);
    if (!arq) {
        cout << "Erro na abertura do arquivo " << Nome << "." << endl;
        return false;
    }

    vector<Ponto> vertices; // Armazena os vértices lidos temporariamente
    vector<Face> facesTemp; // Armazena as faces antes de alocar na classe
    string linha;

    // Lê o arquivo linha por linha
    while (getline(arq, linha)) {
        if (linha.rfind("v ", 0) == 0) { // Verifica se a linha é um vértice
            float x, y, z;
            sscanf(linha.c_str(), "v %f %f %f", &x, &y, &z);
            vertices.emplace_back(Ponto(x, y, z)); // Adiciona o vértice ao vetor
        } else if (linha.rfind("f ", 0) == 0) { // Verifica se a linha é uma face
            int v1, v2, v3;
            sscanf(linha.c_str(), "f %d %d %d", &v1, &v2, &v3);

            // Cria a nova face e adiciona à lista temporária
            facesTemp.emplace_back(
                vertices[v1 - 1], // Os índices de face começam em 1 no formato OBJ
                vertices[v2 - 1],
                vertices[v3 - 1]
            );
        }
    }

    // Armazena as faces no modelo
    nFaces = facesTemp.size();
    faces = new Face[nFaces];
    for (int i = 0; i < nFaces; i++) {
        faces[i] = facesTemp[i];
    }

    arq.close();
    return true;
}

void Modelo3D::DesenharOBJ() const {
    glBegin(GL_TRIANGLES);
    for (unsigned int i = 0; i < nFaces; i++) {
        const Ponto& v1 = faces[i].getP1();
        const Ponto& v2 = faces[i].getP2();
        const Ponto& v3 = faces[i].getP3();

        glVertex3f(v1.x, v1.y, v1.z);
        glVertex3f(v2.x, v2.y, v2.z);
        glVertex3f(v3.x, v3.y, v3.z);
    }
    glEnd();
}


bool Modelo3D::LeObjetoOBJAvancado(const char* Nome) {
    std::ifstream arq(Nome);
    if (!arq.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << Nome << std::endl;
        return false;
    }

    std::vector<Ponto> vertices; // Armazena os vértices
    std::vector<Ponto> normais;  // Armazena as normais
    std::vector<Face> facesTemp; // Armazena as faces antes de transferir

    std::string linha;
    while (std::getline(arq, linha)) {
        std::istringstream iss(linha);

        if (linha.rfind("v ", 0) == 0) { // Detecta vértices
            float x, y, z;
            iss.ignore(2); // Ignora "v "
            iss >> x >> y >> z;
            vertices.emplace_back(Ponto(x, y, z));
        } else if (linha.rfind("vn ", 0) == 0) { // Detecta normais
            float nx, ny, nz;
            iss.ignore(3); // Ignora "vn "
            iss >> nx >> ny >> nz;
            normais.emplace_back(Ponto(nx, ny, nz));
        } else if (linha.rfind("f ", 0) == 0) { // Detecta faces
            int v1, v2, v3;
            int n1 = -1, n2 = -1, n3 = -1;

            // Processa os índices de vértices e normais
            std::string v1n1, v2n2, v3n3;
            iss.ignore(2); // Ignora "f "
            iss >> v1n1 >> v2n2 >> v3n3;

            // Divide os índices de vértices e normais (ex: "1//1")
            sscanf(v1n1.c_str(), "%d//%d", &v1, &n1);
            sscanf(v2n2.c_str(), "%d//%d", &v2, &n2);
            sscanf(v3n3.c_str(), "%d//%d", &v3, &n3);

            // Ajusta índices para começar em 0
            v1--; v2--; v3--;
            n1--; n2--; n3--;

            // Cria a face com vértices e normais (se disponíveis)
            if (n1 >= 0 && n2 >= 0 && n3 >= 0) {
                facesTemp.emplace_back(vertices[v1], vertices[v2], vertices[v3], normais[n1]);
            } else {
                facesTemp.emplace_back(vertices[v1], vertices[v2], vertices[v3]);
            }
        }
    }

    // Transfere os dados para o modelo 3D
    nFaces = facesTemp.size();
    faces = new Face[nFaces];
    for (unsigned int i = 0; i < nFaces; i++) {
        faces[i] = facesTemp[i];
    }

    std::cout << "Modelo carregado com sucesso: " << nFaces << " faces." << std::endl;
    arq.close();
    return true;
}