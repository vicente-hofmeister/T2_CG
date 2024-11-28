#include "Poliedro.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

Poliedro Poliedro::transform(const glm::mat4& matriz) const {
        // Transformar os 8 vértices da bounding box
        Ponto vertices[8] = {
            {min.x, min.y, min.z}, {min.x, min.y, max.z}, {min.x, max.y, min.z}, {min.x, max.y, max.z},
            {max.x, min.y, min.z}, {max.x, min.y, max.z}, {max.x, max.y, min.z}, {max.x, max.y, max.z}
        };

        Ponto minTransformado(FLT_MAX, FLT_MAX, FLT_MAX);
        Ponto maxTransformado(FLT_MIN, FLT_MIN, FLT_MIN);

        for (const auto& vert : vertices) {
            glm::vec4 p(vert.x, vert.y, vert.z, 1.0f); // Coordenada homogênea
            glm::vec4 pTransformado = matriz * p;

        //     std::cout << "Original: (" << vert.x << ", " << vert.y << ", " << vert.z << "), Transformado: (" << pTransformado.x << ", " << pTransformado.y << ", " << pTransformado.z << ")" 
        //   << std::endl;



            // Atualizar min e max com os vértices transformados
            minTransformado.x = std::min(minTransformado.x, pTransformado.x);
            minTransformado.y = std::min(minTransformado.y, pTransformado.y);
            minTransformado.z = std::min(minTransformado.z, pTransformado.z);

            maxTransformado.x = std::max(maxTransformado.x, pTransformado.x);
            maxTransformado.y = std::max(maxTransformado.y, pTransformado.y);
            maxTransformado.z = std::max(maxTransformado.z, pTransformado.z);
        }

        std::cout << "Min Transformado: (" << minTransformado.x << ", " << minTransformado.y << ", " << minTransformado.z << ")" << std::endl;
        std::cout << "Max Transformado: (" << maxTransformado.x << ", " << maxTransformado.y << ", " << maxTransformado.z << ")" << std::endl;    

        return Poliedro(minTransformado, maxTransformado);
    }