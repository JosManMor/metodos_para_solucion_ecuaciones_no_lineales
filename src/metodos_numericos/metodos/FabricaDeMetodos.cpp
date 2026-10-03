#include "metodos_numericos/metodos/FabricaDeMetodos.hpp"

#include "metodos_numericos/validacion/ErrorValidacion.hpp"

#include "metodos_numericos/metodos/Biseccion.hpp"
#include "metodos_numericos/metodos/FalsaPosicion.hpp"
#include "metodos_numericos/metodos/MetodoGrafico.hpp"
#include "metodos_numericos/metodos/NewtonRaphson.hpp"
#include "metodos_numericos/metodos/NewtonRaphsonModificado.hpp"
#include "metodos_numericos/metodos/PuntoFijo.hpp"
#include "metodos_numericos/metodos/Secante.hpp"

namespace mn::metodos {

using mn::core::Funcion;
using mn::core::ParametrosMetodo;

std::unique_ptr<MetodoNumerico> FabricaDeMetodos::crear(TipoMetodo tipo,
                                                         const Funcion& f,
                                                         const std::optional<Funcion>& g,
                                                         const ParametrosMetodo& params) {
    switch (tipo) {
        case TipoMetodo::GRAFICO:
            return std::make_unique<MetodoGrafico>(f, params);
        case TipoMetodo::BISECCION:
            return std::make_unique<Biseccion>(f, params);
        case TipoMetodo::FALSA_POSICION:
            return std::make_unique<FalsaPosicion>(f, params);
        case TipoMetodo::PUNTO_FIJO:
            if (!g.has_value()) {
                throw mn::validacion::ErrorValidacion(
                    "El método de punto fijo requiere la función g(x) despejada de f(x) = 0.");
            }
            return std::make_unique<PuntoFijo>(*g, params);
        case TipoMetodo::NEWTON_RAPHSON:
            return std::make_unique<NewtonRaphson>(f, params);
        case TipoMetodo::SECANTE:
            return std::make_unique<Secante>(f, params);
        case TipoMetodo::NEWTON_RAPHSON_MODIFICADO:
            return std::make_unique<NewtonRaphsonModificado>(f, params);
    }
    throw mn::validacion::ErrorValidacion("Método numérico no reconocido.");
}

std::string FabricaDeMetodos::nombreMetodo(TipoMetodo tipo) {
    switch (tipo) {
        case TipoMetodo::GRAFICO: return "Método gráfico";
        case TipoMetodo::BISECCION: return "Bisección";
        case TipoMetodo::FALSA_POSICION: return "Falsa posición";
        case TipoMetodo::PUNTO_FIJO: return "Iteración de punto fijo";
        case TipoMetodo::NEWTON_RAPHSON: return "Newton-Raphson";
        case TipoMetodo::SECANTE: return "Secante";
        case TipoMetodo::NEWTON_RAPHSON_MODIFICADO: return "Newton-Raphson modificado (raíces múltiples)";
    }
    return "Desconocido";
}

} // namespace mn::metodos
