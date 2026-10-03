#pragma once

#include "metodos_numericos/core/Funcion.hpp"
#include "metodos_numericos/metodos/MetodoNumerico.hpp"

namespace mn::metodos {

// Recibe g(x), la forma despejada de f(x) = 0 como x = g(x).
class PuntoFijo : public MetodoNumerico {
public:
    PuntoFijo(mn::core::Funcion g, mn::core::ParametrosMetodo params);

    mn::core::ResultadoFinal ejecutar() override;
    const std::vector<mn::core::IteracionResultado>& obtenerTabla() const override;
    std::vector<std::string> columnas() const override;
    std::string nombre() const override { return "Iteración de punto fijo"; }

private:
    mn::core::Funcion g_;
    mn::core::ParametrosMetodo params_;
    std::vector<mn::core::IteracionResultado> tabla_;
};

} // namespace mn::metodos
