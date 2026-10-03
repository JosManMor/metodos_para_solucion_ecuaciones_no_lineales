#pragma once

#include <memory>
#include <optional>

#include "metodos_numericos/core/Funcion.hpp"
#include "metodos_numericos/metodos/MetodoNumerico.hpp"
#include "metodos_numericos/metodos/TipoMetodo.hpp"

namespace mn::metodos {

// Dado el tipo de método elegido por el usuario, construye la instancia
// concreta correspondiente detrás de la interfaz MetodoNumerico.
class FabricaDeMetodos {
public:
    // `g` solo se usa para TipoMetodo::PUNTO_FIJO (la forma x = g(x));
    // en el resto de los métodos se ignora.
    static std::unique_ptr<MetodoNumerico> crear(TipoMetodo tipo,
                                                  const mn::core::Funcion& f,
                                                  const std::optional<mn::core::Funcion>& g,
                                                  const mn::core::ParametrosMetodo& params);

    static std::string nombreMetodo(TipoMetodo tipo);
};

} // namespace mn::metodos
