#pragma once

#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace mn::core {

// Parámetros de entrada que puede requerir cualquiera de los métodos.
// Cada método solo usa el subconjunto de campos que le corresponde;
// el resto queda en su valor por omisión (ver Validador).
struct ParametrosMetodo {
    std::optional<double> a;                 // extremo izquierdo del intervalo
    std::optional<double> b;                 // extremo derecho del intervalo
    std::optional<double> x0;                // valor inicial 1
    std::optional<double> x1;                // valor inicial 2 (secante)
    std::optional<int> multiplicidad;        // raíces múltiples (Newton modificado)
    double tolerancia = 1e-6;
    int iteracionesMaximas = 100;
    // criterio de paro: "error_absoluto" (|x_{i+1}-x_i| o similar) o "f(x)" (|f(xi)| < tol)
    std::string criterioParo = "error_absoluto";
};

// Una fila de la tabla de iteraciones. Las columnas varían según el método,
// por lo que se guardan como pares (nombre_columna, valor) en el orden en
// que deben mostrarse.
struct IteracionResultado {
    int numero = 0;
    std::vector<std::pair<std::string, double>> valores;
};

// Resumen del resultado de un método al terminar (con éxito o no).
struct ResultadoFinal {
    bool exito = false;
    double raiz = 0.0;
    int iteraciones = 0;
    double errorFinal = 0.0;
    std::string criterioParoAlcanzado;
    std::string mensaje;
};

} // namespace mn::core
