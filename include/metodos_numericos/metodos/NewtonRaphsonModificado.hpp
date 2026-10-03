#pragma once

#include "metodos_numericos/core/Funcion.hpp"
#include "metodos_numericos/metodos/MetodoNumerico.hpp"

namespace mn::metodos {

// Variante de Newton-Raphson para raíces múltiples. Si el usuario conoce la
// multiplicidad m, se usa x_{i+1} = x_i - m f(x_i)/f'(x_i); si no, se usa la
// fórmula general que recupera convergencia cuadrática mediante f''(x).
class NewtonRaphsonModificado : public MetodoNumerico {
public:
    NewtonRaphsonModificado(mn::core::Funcion f, mn::core::ParametrosMetodo params);

    mn::core::ResultadoFinal ejecutar() override;
    const std::vector<mn::core::IteracionResultado>& obtenerTabla() const override;
    std::vector<std::string> columnas() const override;
    std::string nombre() const override { return "Newton-Raphson modificado (raíces múltiples)"; }

private:
    mn::core::Funcion f_;
    mn::core::ParametrosMetodo params_;
    std::vector<mn::core::IteracionResultado> tabla_;
};

} // namespace mn::metodos
