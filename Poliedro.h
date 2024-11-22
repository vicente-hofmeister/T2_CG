#ifndef Poliedro_hpp
#define Poliedro_hpp

#include "Ponto.h"

class Poliedro
{
    Ponto min,max;

public:
    Poliedro();

    Poliedro(const Ponto& min, const Ponto& max);

    Ponto getMin() const;
    Ponto getMax() const;

    void setMin(const Ponto& ponto);
    void setMax(const Ponto& ponto);
};

#endif