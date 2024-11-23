#include "Poliedro.h"

Poliedro::Poliedro() : min({0, 0, 0}), max({1, 1, 1}) {}

Poliedro::Poliedro(const Ponto& min, const Ponto& max) : min(min), max(max) {}

Ponto Poliedro::getMin() const {
    return min;
}

Ponto Poliedro::getMax() const {
    return max;
}

void Poliedro::setMin(const Ponto& ponto) {
    min = ponto;
}

void Poliedro::setMax(const Ponto& ponto) {
    max = ponto;
}