#include "metodos_numericos/metodos/FalsaPosicion.hpp"

#include <cmath>

namespace mn::metodos {

using mn::core::IteracionResultado;
using mn::core::ResultadoFinal;

FalsaPosicion::FalsaPosicion(mn::core::Funcion f, mn::core::ParametrosMetodo params)
    : f_(std::move(f)), params_(std::move(params)) {}

std::vector<std::string> FalsaPosicion::columnas() const {
    return {"i", "a", "b", "xr", "f(xr)", "error"};
}

const std::vector<IteracionResultado>& FalsaPosicion::obtenerTabla() const { return tabla_; }

ResultadoFinal FalsaPosicion::ejecutar() {
    tabla_.clear();
    double a = *params_.a;
    double b = *params_.b;
    double fa = f_.evaluar(a);
    double fb = f_.evaluar(b);

    if (fa == 0.0) return {true, a, 0, 0.0, "f(a) = 0 (raíz exacta)", "Raíz exacta encontrada en a."};
    if (fb == 0.0) return {true, b, 0, 0.0, "f(b) = 0 (raíz exacta)", "Raíz exacta encontrada en b."};

    double xrAnterior = a;
    for (int i = 1; i <= params_.iteracionesMaximas; ++i) {
        double xr = b - fb * (a - b) / (fa - fb);
        double fxr = f_.evaluar(xr);
        double error = (i == 1) ? std::fabs(b - a) : std::fabs(xr - xrAnterior);

        IteracionResultado fila;
        fila.numero = i;
        fila.valores = {{"a", a}, {"b", b}, {"xr", xr}, {"f(xr)", fxr}, {"error", error}};
        tabla_.push_back(fila);

        if (fxr == 0.0) {
            return {true, xr, i, 0.0, "f(xr) = 0 (raíz exacta)", "Raíz exacta encontrada."};
        }
        if (error < params_.tolerancia) {
            return {true, xr, i, error, "error < tolerancia", "Convergencia alcanzada."};
        }

        if (fa * fxr < 0.0) {
            b = xr;
            fb = fxr;
        } else {
            a = xr;
            fa = fxr;
        }
        xrAnterior = xr;
    }

    return {false, xrAnterior, params_.iteracionesMaximas, std::fabs(b - a),
            "máximo de iteraciones alcanzado",
            "No se alcanzó la tolerancia solicitada dentro del máximo de iteraciones."};
}

} // namespace mn::metodos
