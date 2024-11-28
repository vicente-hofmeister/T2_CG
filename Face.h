#ifndef FACE_H
#define FACE_H

#include "Ponto.h"

class Face {
private:
    Ponto P1, P2, P3;          // Vértices da face
    Ponto normal;              // Normal da face (se presente)
    unsigned int cor;          // Cor da face (opcional)
    int grupo;                 // Grupo ou material associado à face (opcional)

public:
    Face() : cor(0xFFFFFF), grupo(-1) {} // Construtor padrão
    Face(const Ponto& p1, const Ponto& p2, const Ponto& p3)
        : P1(p1), P2(p2), P3(p3), cor(0xFFFFFF), grupo(-1) {}

    Face(const Ponto& p1, const Ponto& p2, const Ponto& p3, const Ponto& n)
        : P1(p1), P2(p2), P3(p3), normal(n), cor(0xFFFFFF), grupo(-1) {}

    void setP1(const Ponto& p) { P1 = p; }
    void setP2(const Ponto& p) { P2 = p; }
    void setP3(const Ponto& p) { P3 = p; }
    void setNormal(const Ponto& n) { normal = n; }
    void setCor(unsigned int c) { cor = c; }
    void setGrupo(int g) { grupo = g; }

    const Ponto& getP1() const { return P1; }
    const Ponto& getP2() const { return P2; }
    const Ponto& getP3() const { return P3; }
    const Ponto& getNormal() const { return normal; }
    unsigned int getCor() const { return cor; }
    int getGrupo() const { return grupo; }
};

#endif // FACE_H