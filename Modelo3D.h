#ifndef MODELO3D_H
#define MODELO3D_H

#include <string>
#include "Ponto.h"
#include "Face.h"

class Modelo3D {
private:
    Face *faces;
    unsigned int nFaces;

public:
    Modelo3D() = default;

    bool LeObjetoSimples(char *Nome);

    bool LeObjetoCompleto(const char *Nome);

    void DesenharSimples() const;

    void DesenharCompleto() const;

    bool LeObjetoOBJ(char* Nome);
    
    void DesenharOBJ() const;

    bool LeObjetoOBJAvancado(const char* Nome);
};

#endif 