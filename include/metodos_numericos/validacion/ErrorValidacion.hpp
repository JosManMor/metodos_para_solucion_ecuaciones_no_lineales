#pragma once

#include <stdexcept>
#include <string>

namespace mn::validacion {

// Representa una condición de entrada que impide ejecutar un método
// (dato faltante, intervalo sin cambio de signo, tolerancia negativa, etc.).
// Se distingue de los errores de dominio/sintaxis de Funcion porque estos
// se detectan ANTES de iniciar el cálculo, a partir de los parámetros.
class ErrorValidacion : public std::runtime_error {
public:
    explicit ErrorValidacion(const std::string& mensaje) : std::runtime_error(mensaje) {}
};

} // namespace mn::validacion
