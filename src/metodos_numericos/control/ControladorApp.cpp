#include "metodos_numericos/control/ControladorApp.hpp"

#include "metodos_numericos/core/Funcion.hpp"
#include "metodos_numericos/grafica/GraficadorAscii.hpp"
#include "metodos_numericos/metodos/FabricaDeMetodos.hpp"
#include "metodos_numericos/validacion/Validador.hpp"

namespace mn::control {

using mn::core::Funcion;

RespuestaCalculo ControladorApp::ejecutarCalculo(const SolicitudCalculo& solicitud) {
    RespuestaCalculo respuesta;
    try {
        Funcion f(solicitud.expresionF);

        std::optional<Funcion> g;
        if (solicitud.expresionG.has_value()) {
            g = Funcion(*solicitud.expresionG);
        }

        mn::validacion::Validador::validar(solicitud.tipo, f, solicitud.params);

        auto metodo = mn::metodos::FabricaDeMetodos::crear(solicitud.tipo, f, g, solicitud.params);
        respuesta.resultado = metodo->ejecutar();
        respuesta.tabla = metodo->obtenerTabla();
        respuesta.columnas = metodo->columnas();
        respuesta.nombreMetodo = metodo->nombre();
        respuesta.exito = true;
    } catch (const std::exception& e) {
        respuesta.exito = false;
        respuesta.mensajeError = e.what();
    }
    return respuesta;
}

std::string ControladorApp::generarGrafica(const std::string& expresionF, double xMin, double xMax,
                                            std::optional<double> raiz) const {
    try {
        Funcion f(expresionF);
        mn::grafica::GraficadorAscii graficador;
        return graficador.graficar(f, xMin, xMax, raiz);
    } catch (const std::exception& e) {
        return std::string("No se pudo generar la gráfica: ") + e.what();
    }
}

} // namespace mn::control
