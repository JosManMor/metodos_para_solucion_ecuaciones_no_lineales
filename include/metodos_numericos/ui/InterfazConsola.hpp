#pragma once

#include "metodos_numericos/control/ControladorApp.hpp"

namespace mn::ui {

// Vista de consola: la única parte del sistema que lee de std::cin y escribe
// en std::cout. Solo conoce a ControladorApp (nunca a los métodos concretos,
// al validador, ni al parser de funciones directamente).
class InterfazConsola {
public:
    void ejecutar();

private:
    mn::control::ControladorApp controlador_;

    void mostrarBienvenida() const;
    void mostrarAyudaSintaxis() const;

    // Devuelve std::nullopt si el usuario escribe "salir".
    std::optional<mn::control::SolicitudCalculo> capturarSolicitud();

    void mostrarRespuesta(const mn::control::RespuestaCalculo& respuesta,
                           const std::string& expresionF);

    void mostrarTabla(const mn::control::RespuestaCalculo& respuesta) const;

    void ofrecerGrafica(const std::string& expresionF, const mn::core::ResultadoFinal& resultado,
                         const mn::core::ParametrosMetodo& params);

    // Utilidades de lectura con reintento ante entradas inválidas.
    std::string leerLinea(const std::string& indicacion) const;
    double leerDouble(const std::string& indicacion) const;
    int leerEntero(const std::string& indicacion) const;
    bool leerSiNo(const std::string& indicacion) const;
};

} // namespace mn::ui
