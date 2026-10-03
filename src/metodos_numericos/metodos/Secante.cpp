#include "metodos_numericos/metodos/Secante.hpp"

#include <cmath>

namespace mn::metodos {

using mn::core::IteracionResultado;
using mn::core::ResultadoFinal;

Secante::Secante(mn::core::Funcion f, mn::core::ParametrosMetodo params)
    : f_(std::move(f)), params_(std::move(params)) {}

std::vector<std::string> Secante::columnas() const {
    return {"i", "xi-1", "xi", "f(xi-1)", "f(xi)", "xi+1", "error"};
}

const std::vector<IteracionResultado>& Secante::obtenerTabla() const { return tabla_; }

ResultadoFinal Secante::ejecutar() {
    tabla_.clear();
    double xAnterior = *params_.x0;
    double xActual = *params_.x1;

    for (int i = 1; i <= params_.iteracionesMaximas; ++i) {
        double fAnterior = f_.evaluar(xAnterior);
        double fActual = f_.evaluar(xActual);
        double denominador = fActual - fAnterior;

        if (std::fabs(denominador) < 1e-14) {
            return {false, xActual, i, std::fabs(fActual), "f(xi) - f(xi-1) ≈ 0",
                    "La diferencia f(xi) - f(xi-1) es prácticamente cero: división entre cero. "
                    "Prueba otros valores iniciales."};
        }

        double xSiguiente = xActual - fActual * (xActual - xAnterior) / denominador;
        double error = std::fabs(xSiguiente - xActual);

        IteracionResultado fila;
        fila.numero = i;
        fila.valores = {{"xi-1", xAnterior}, {"xi", xActual}, {"f(xi-1)", fAnterior},
                         {"f(xi)", fActual}, {"xi+1", xSiguiente}, {"error", error}};
        tabla_.push_back(fila);

        if (error < params_.tolerancia) {
            return {true, xSiguiente, i, error, "error < tolerancia", "Convergencia alcanzada."};
        }
        xAnterior = xActual;
        xActual = xSiguiente;
    }

    return {false, xActual, params_.iteracionesMaximas, 0.0,
            "máximo de iteraciones alcanzado",
            "No se alcanzó la tolerancia solicitada dentro del máximo de iteraciones."};
}

} // namespace mn::metodos
