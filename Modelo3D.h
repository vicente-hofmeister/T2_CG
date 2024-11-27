#ifndef MODELO3D_H
#define MODELO3D_H

#include <vector>
#include <string>
#include "Ponto.h"

class Modelo3D {
private:
    std::vector<Ponto> vertices; 

public:
    Modelo3D() = default;

    bool carregarModelo(const std::string& caminhoArquivo);

    void desenhar() const;

    const std::vector<Ponto>& getVertices() const;
};

#endif 