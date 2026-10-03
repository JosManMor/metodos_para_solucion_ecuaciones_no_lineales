#pragma once

#include <optional>
#include <string>
#include <vector>

#include "metodos_numericos/core/Tipos.hpp"
#include "metodos_numericos/metodos/TipoMetodo.hpp"

namespace mn::control {

// Lo que la interfaz de usuario reúne del usuario para pedir un cálculo.
struct SolicitudCalculo {
    std::string expresionF;
    std::optional<std::string> expresionG; // solo para punto fijo: x = g(x)
    mn::metodos::TipoMetodo tipo;
    mn::core::ParametrosMetodo params;
};

// Lo que el controlador devuelve a la interfaz tras intentar el cálculo.
struct RespuestaCalculo {
    bool exito = false;
    std::string mensajeError;                 // solo válido si exito == false
    mn::core::ResultadoFinal resultado;
    std::vector<mn::core::IteracionResultado> tabla;
    std::vector<std::string> columnas;
    std::string nombreMetodo;
};

// Orquesta el flujo completo: valida, construye el método vía la fábrica,
// lo ejecuta, y devuelve resultados listos para mostrarse. No conoce nada
// sobre cómo se presenta la información (eso es responsabilidad de la Vista).
class ControladorApp {
public:
    RespuestaCalculo ejecutarCalculo(const SolicitudCalculo& solicitud);

    // Genera la representación de texto de f(x) para mostrarla en consola.
    std::string generarGrafica(const std::string& expresionF, double xMin, double xMax,
                                std::optional<double> raiz) const;
};

} // namespace mn::control
