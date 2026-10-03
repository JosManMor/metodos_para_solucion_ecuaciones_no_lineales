#include "metodos_numericos/metodos/NewtonRaphson.hpp"

#include <cmath>

namespace mn::metodos {

using mn::core::IteracionResultado;
using mn::core::ResultadoFinal;

NewtonRaphson::NewtonRaphson(mn::core::Funcion f, mn::core::ParametrosMetodo params)
    : f_(std::move(f)), params_(std::move(params)) {}

std::vector<std::string> NewtonRaphson::columnas() const {
    return {"i", "xi", "f(xi)", "f'(xi)", "xi+1", "error"};
}

const std::vector<IteracionResultado>& NewtonRaphson::obtenerTabla() const { return tabla_; }

ResultadoFinal NewtonRaphson::ejecutar() {
    tabla_.clear();
    double xi = *params_.x0;

    for (int i = 1; i <= params_.iteracionesMaximas; ++i) {
        double fxi = f_.evaluar(xi);
        double dfxi = f_.derivada(xi);

        if (std::fabs(dfxi) < 1e-12) {
            return {false, xi, i, std::fabs(fxi), "derivada prácticamente nula",
                    "f'(xi) es prácticamente cero en xi = " + std::to_string(xi) +
                        "; Newton-Raphson no puede continuar. Prueba otro valor inicial."};
        }

        double xi1 = xi - fxi / dfxi;
        double error = std::fabs(xi1 - xi);

        IteracionResultado fila;
        fila.numero = i;
        fila.valores = {{"xi", xi}, {"f(xi)", fxi}, {"f'(xi)", dfxi}, {"xi+1", xi1}, {"error", error}};
        tabla_.push_back(fila);

        if (error < params_.tolerancia) {
            return {true, xi1, i, error, "error < tolerancia", "Convergencia alcanzada."};
        }
        xi = xi1;
    }

    return {false, xi, params_.iteracionesMaximas, 0.0,
            "máximo de iteraciones alcanzado",
            "No se alcanzó la tolerancia solicitada dentro del máximo de iteraciones."};
}

} // namespace mn::metodos
