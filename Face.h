#ifndef FACE_H
#define FACE_H

#include "Ponto.h"

class Face {
private:
    Ponto P1, P2, P3;
public:
    Face() = default;

    Face(Ponto P1, Ponto P2, Ponto P3);

    Ponto getP1();

    Ponto getP2();

    Ponto getP3();

    void setP1(Ponto P1);

    void setP2(Ponto P2);

    void setP3(Ponto P3);
};

#endif