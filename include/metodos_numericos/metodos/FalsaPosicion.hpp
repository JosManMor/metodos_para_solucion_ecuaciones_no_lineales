#pragma once

#include "metodos_numericos/core/Funcion.hpp"
#include "metodos_numericos/metodos/MetodoNumerico.hpp"

namespace mn::metodos {

class FalsaPosicion : public MetodoNumerico {
public:
    FalsaPosicion(mn::core::Funcion f, mn::core::ParametrosMetodo params);

    mn::core::ResultadoFinal ejecutar() override;
    const std::vector<mn::core::IteracionResultado>& obtenerTabla() const override;
    std::vector<std::string> columnas() const override;
    std::string nombre() const override { return "Falsa posición"; }

private:
    mn::core::Funcion f_;
    mn::core::ParametrosMetodo params_;
    std::vector<mn::core::IteracionResultado> tabla_;
};

} // namespace mn::metodos
