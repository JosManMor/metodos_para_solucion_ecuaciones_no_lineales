#include "metodos_numericos/metodos/MetodoGrafico.hpp"

#include <cmath>

namespace mn::metodos {

using mn::core::IteracionResultado;
using mn::core::ResultadoFinal;

MetodoGrafico::MetodoGrafico(mn::core::Funcion f, mn::core::ParametrosMetodo params, int muestras)
    : f_(std::move(f)), params_(std::move(params)), muestras_(muestras) {}

std::vector<std::string> MetodoGrafico::columnas() const {
    return {"i", "x_izq", "x_der", "f(x_izq)", "f(x_der)"};
}

const std::vector<IteracionResultado>& MetodoGrafico::obtenerTabla() const { return tabla_; }

ResultadoFinal MetodoGrafico::ejecutar() {
    tabla_.clear();
    double a = *params_.a;
    double b = *params_.b;
    double paso = (b - a) / static_cast<double>(muestras_);

    int encontrados = 0;
    double primeraRaizAprox = 0.0;
    double primerAncho = 0.0;

    double xIzq = a;
    double fIzq = f_.evaluar(xIzq);
    for (int i = 1; i <= muestras_; ++i) {
        double xDer = a + i * paso;
        double fDer = f_.evaluar(xDer);

        if (fIzq == 0.0 || fDer == 0.0 || fIzq * fDer < 0.0) {
            ++encontrados;
            IteracionResultado fila;
            fila.numero = encontrados;
            fila.valores = {{"x_izq", xIzq}, {"x_der", xDer}, {"f(x_izq)", fIzq}, {"f(x_der)", fDer}};
            tabla_.push_back(fila);
            if (encontrados == 1) {
                primeraRaizAprox = (xIzq + xDer) / 2.0;
                primerAncho = std::fabs(xDer - xIzq);
            }
        }
        xIzq = xDer;
        fIzq = fDer;
    }

    if (encontrados == 0) {
        return {false, 0.0, muestras_, 0.0, "sin cambios de signo detectados",
                "No se detectó ningún cambio de signo en [" + std::to_string(a) + ", " +
                    std::to_string(b) + "]. Prueba otro rango."};
    }

    return {true, primeraRaizAprox, encontrados, primerAncho,
            "exploración completada",
            "Se detectaron " + std::to_string(encontrados) +
                " subintervalo(s) con cambio de signo. Usa alguno de ellos como punto de partida "
                "para los métodos de intervalo o abiertos."};
}

} // namespace mn::metodos
