#include "Face.h"

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

Ponto P1, P2, P3;

Face::Face(Ponto P1, Ponto P2, Ponto P3) {
    this->P1 = P1;
    this->P2 = P2;
    this->P3 = P3;
}

Ponto Face::getP1() {
    return P1;
}

Ponto Face::getP2() {
    return P2;
}

Ponto Face::getP3() {
    return P3;
}

void Face::setP1(Ponto P1) {
    this->P1 = P1;
}

void Face::setP2(Ponto P2) {
    this->P2 = P2;
}

void Face::setP3(Ponto P3) {
    this->P3 = P3;
}