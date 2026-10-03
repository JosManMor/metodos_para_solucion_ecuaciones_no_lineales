#pragma once

#include "metodos_numericos/core/Funcion.hpp"
#include "metodos_numericos/metodos/MetodoNumerico.hpp"

namespace mn::metodos {

// No busca una raíz por iteración: muestrea f(x) en [a,b] y reporta los
// subintervalos donde hay cambio de signo, como apoyo para elegir valores
// iniciales de los demás métodos. La gráfica real se dibuja en GraficadorAscii.
class MetodoGrafico : public MetodoNumerico {
public:
    MetodoGrafico(mn::core::Funcion f, mn::core::ParametrosMetodo params, int muestras = 200);

    mn::core::ResultadoFinal ejecutar() override;
    const std::vector<mn::core::IteracionResultado>& obtenerTabla() const override;
    std::vector<std::string> columnas() const override;
    std::string nombre() const override { return "Método gráfico"; }

private:
    mn::core::Funcion f_;
    mn::core::ParametrosMetodo params_;
    int muestras_;
    std::vector<mn::core::IteracionResultado> tabla_;
};

} // namespace mn::metodos
