#include "metodos_numericos/metodos/NewtonRaphsonModificado.hpp"

#include <cmath>

namespace mn::metodos {

using mn::core::IteracionResultado;
using mn::core::ResultadoFinal;

NewtonRaphsonModificado::NewtonRaphsonModificado(mn::core::Funcion f, mn::core::ParametrosMetodo params)
    : f_(std::move(f)), params_(std::move(params)) {}

std::vector<std::string> NewtonRaphsonModificado::columnas() const {
    return {"i", "xi", "f(xi)", "f'(xi)", "f''(xi)", "xi+1", "error"};
}

const std::vector<IteracionResultado>& NewtonRaphsonModificado::obtenerTabla() const { return tabla_; }

ResultadoFinal NewtonRaphsonModificado::ejecutar() {
    tabla_.clear();
    double xi = *params_.x0;
    const bool multiplicidadConocida = params_.multiplicidad.has_value();
    const double m = multiplicidadConocida ? static_cast<double>(*params_.multiplicidad) : 0.0;

    for (int i = 1; i <= params_.iteracionesMaximas; ++i) {
        double fxi = f_.evaluar(xi);
        double dfxi = f_.derivada(xi);
        double d2fxi = f_.segundaDerivada(xi);

        double xi1;
        double denominador;
        if (multiplicidadConocida) {
            denominador = dfxi;
            if (std::fabs(denominador) < 1e-12) {
                return {false, xi, i, std::fabs(fxi), "f'(xi) prácticamente nula",
                        "f'(xi) es prácticamente cero: no se puede continuar con la multiplicidad declarada."};
            }
            xi1 = xi - m * fxi / denominador;
        } else {
            denominador = dfxi * dfxi - fxi * d2fxi;
            if (std::fabs(denominador) < 1e-12) {
                return {false, xi, i, std::fabs(fxi), "denominador prácticamente nulo",
                        "El denominador [f'(xi)]^2 - f(xi)f''(xi) es prácticamente cero: no se puede continuar."};
            }
            xi1 = xi - (fxi * dfxi) / denominador;
        }

        double error = std::fabs(xi1 - xi);

        IteracionResultado fila;
        fila.numero = i;
        fila.valores = {{"xi", xi}, {"f(xi)", fxi}, {"f'(xi)", dfxi},
                         {"f''(xi)", d2fxi}, {"xi+1", xi1}, {"error", error}};
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
