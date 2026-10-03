#pragma once

#include <optional>
#include <string>

#include "metodos_numericos/core/Funcion.hpp"

namespace mn::grafica {

// Genera una representación de texto de f(x) para mostrarse dentro de la
// propia terminal de la aplicación (sin depender de ningún programa externo).
// Dibuja los ejes, la curva y, si se proporciona, marca la raíz aproximada.
class GraficadorAscii {
public:
    GraficadorAscii(int ancho = 76, int alto = 23);

    std::string graficar(const mn::core::Funcion& f,
                          double xMin,
                          double xMax,
                          std::optional<double> raiz = std::nullopt) const;

private:
    int ancho_;
    int alto_;
};

} // namespace mn::grafica
