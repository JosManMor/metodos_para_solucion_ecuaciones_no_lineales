#pragma once

#include <string>
#include <vector>

#include "metodos_numericos/core/Tipos.hpp"

namespace mn::metodos {

// Interfaz común que implementan los seis métodos de la Unidad 2 y la
// variante de Newton-Raphson modificado. El ControladorApp / la interfaz
// de usuario solo conocen esta interfaz (polimorfismo), nunca las clases
// concretas directamente.
class MetodoNumerico {
public:
    virtual ~MetodoNumerico() = default;

    // Ejecuta el proceso iterativo completo y construye la tabla interna.
    // Los parámetros ya deben haber sido validados por mn::validacion::Validador.
    virtual mn::core::ResultadoFinal ejecutar() = 0;

    // Tabla de iteraciones generada por la última llamada a ejecutar().
    virtual const std::vector<mn::core::IteracionResultado>& obtenerTabla() const = 0;

    // Encabezados de columnas, en el orden en que deben imprimirse.
    virtual std::vector<std::string> columnas() const = 0;

    virtual std::string nombre() const = 0;
};

} // namespace mn::metodos
