#include "metodos_numericos/metodos/PuntoFijo.hpp"

#include <cmath>
#include <limits>

namespace mn::metodos {

using mn::core::IteracionResultado;
using mn::core::ResultadoFinal;

PuntoFijo::PuntoFijo(mn::core::Funcion g, mn::core::ParametrosMetodo params)
    : g_(std::move(g)), params_(std::move(params)) {}

std::vector<std::string> PuntoFijo::columnas() const { return {"i", "xi", "g(xi)", "error"}; }

const std::vector<IteracionResultado>& PuntoFijo::obtenerTabla() const { return tabla_; }

ResultadoFinal PuntoFijo::ejecutar() {
    tabla_.clear();
    double xi = *params_.x0;
    double errorAnterior = std::numeric_limits<double>::infinity();

    for (int i = 1; i <= params_.iteracionesMaximas; ++i) {
        double xi1 = g_.evaluar(xi);
        double error = std::fabs(xi1 - xi);

        IteracionResultado fila;
        fila.numero = i;
        fila.valores = {{"xi", xi}, {"g(xi)", xi1}, {"error", error}};
        tabla_.push_back(fila);

        if (!std::isfinite(xi1) || error > 1e12) {
            return {false, xi, i, error, "divergencia detectada",
                    "El método diverge con esta g(x) y x0: |g'(x)| > 1 cerca del punto inicial. "
                    "Intenta otra forma de despeje x = g(x)."};
        }
        if (error < params_.tolerancia) {
            return {true, xi1, i, error, "error < tolerancia", "Convergencia alcanzada."};
        }
        errorAnterior = error;
        xi = xi1;
    }

    return {false, xi, params_.iteracionesMaximas, errorAnterior,
            "máximo de iteraciones alcanzado",
            "No se alcanzó la tolerancia solicitada dentro del máximo de iteraciones."};
}

} // namespace mn::metodos
