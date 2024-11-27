#ifndef FACE_H
#define FACE_H

#include "Ponto.h"

class Face {
private:
    Ponto P1, P2, P3;
    unsigned int cor;
    int grupo;
public:
    Face() = default;

    Face(const Ponto& p1, const Ponto& p2, const Ponto& p3)
    : P1(p1), P2(p2), P3(p3), cor(0xFFFFFF), grupo(-1) {
    std::cout << "Criando Face: P1(" << P1.x << ", " << P1.y << ", " << P1.z 
              << "), P2(" << P2.x << ", " << P2.y << ", " << P2.z 
              << "), P3(" << P3.x << ", " << P3.y << ", " << P3.z << ")\n";
    }

    Face(Ponto P1, Ponto P2, Ponto P3, unsigned int cor, int grupo);

    Ponto getP1();

    Ponto getP2();

    Ponto getP3();

    unsigned int getCor();

    int getGrupo();

    void setP1(Ponto P1);

    void setP2(Ponto P2);

    void setP3(Ponto P3);

    void setCor(unsigned int cor);

    void setGrupo(int grupo);
};

#endif