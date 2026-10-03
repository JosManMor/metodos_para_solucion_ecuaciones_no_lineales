#include "metodos_numericos/ui/InterfazConsola.hpp"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace mn::ui {

using mn::control::RespuestaCalculo;
using mn::control::SolicitudCalculo;
using mn::core::ParametrosMetodo;
using mn::core::ResultadoFinal;
using mn::metodos::TipoMetodo;

namespace {

// Señal interna para terminar la aplicación con limpieza (EOF o "salir").
struct SalirAplicacion {};

std::string aMinusculasRecortado(const std::string& s) {
    std::string r = s;
    r.erase(0, r.find_first_not_of(" \t\r\n"));
    auto fin = r.find_last_not_of(" \t\r\n");
    if (fin != std::string::npos) r.erase(fin + 1);
    std::transform(r.begin(), r.end(), r.begin(), [](unsigned char c) { return std::tolower(c); });
    return r;
}

} // namespace

std::string InterfazConsola::leerLinea(const std::string& indicacion) const {
    std::cout << indicacion;
    std::string linea;
    if (!std::getline(std::cin, linea)) {
        throw SalirAplicacion{};
    }
    return linea;
}

double InterfazConsola::leerDouble(const std::string& indicacion) const {
    while (true) {
        std::string linea = leerLinea(indicacion);
        std::string limpia = aMinusculasRecortado(linea);
        if (limpia == "salir") throw SalirAplicacion{};
        try {
            size_t pos = 0;
            double v = std::stod(linea, &pos);
            while (pos < linea.size() && std::isspace(static_cast<unsigned char>(linea[pos]))) ++pos;
            if (pos != linea.size()) throw std::invalid_argument("sobran caracteres");
            return v;
        } catch (...) {
            std::cout << "  -> Entrada no numérica. Escribe un número (ejemplo: 1.5 o -2.3).\n";
        }
    }
}

int InterfazConsola::leerEntero(const std::string& indicacion) const {
    while (true) {
        std::string linea = leerLinea(indicacion);
        std::string limpia = aMinusculasRecortado(linea);
        if (limpia == "salir") throw SalirAplicacion{};
        try {
            size_t pos = 0;
            int v = std::stoi(linea, &pos);
            while (pos < linea.size() && std::isspace(static_cast<unsigned char>(linea[pos]))) ++pos;
            if (pos != linea.size()) throw std::invalid_argument("sobran caracteres");
            return v;
        } catch (...) {
            std::cout << "  -> Entrada no válida. Escribe un número entero (ejemplo: 50).\n";
        }
    }
}

bool InterfazConsola::leerSiNo(const std::string& indicacion) const {
    while (true) {
        std::string linea = aMinusculasRecortado(leerLinea(indicacion));
        if (linea == "salir") throw SalirAplicacion{};
        if (linea == "s" || linea == "si" || linea == "sí" || linea == "y" || linea == "yes") return true;
        if (linea == "n" || linea == "no") return false;
        std::cout << "  -> Responde 's' (sí) o 'n' (no).\n";
    }
}

void InterfazConsola::mostrarAyudaSintaxis() const {
    std::cout <<
        "Sintaxis admitida para f(x):\n"
        "  Operadores: + - * / ^        Paréntesis: ( )\n"
        "  Funciones:  sin cos tan asin acos atan sinh cosh tanh exp ln log sqrt abs\n"
        "  Constantes: pi, e             Variable:  x\n"
        "  Ejemplos:   x^3 - 4*x - 9     2*exp(x) - 5     ln(x) - x + 2\n"
        "              sin(x) - 0.5*x    x^2*exp(-x) - 1\n"
        "  (En cualquier momento puedes escribir 'salir' para cerrar la aplicación)\n\n";
}

void InterfazConsola::mostrarBienvenida() const {
    std::cout <<
        "================================================================\n"
        " Aplicacion de Metodos Numericos - Unidad 2 (Ecuaciones no lineales)\n"
        " Estudiante: Carrillo Valencia Ruth | Grupo: AM3B | Lenguaje: C++\n"
        "================================================================\n\n";
    mostrarAyudaSintaxis();
}

std::optional<SolicitudCalculo> InterfazConsola::capturarSolicitud() {
    std::string expresionF = leerLinea("f(x) = ");
    if (aMinusculasRecortado(expresionF) == "salir") return std::nullopt;

    std::cout <<
        "\nSelecciona el metodo numerico:\n"
        "  1. Metodo grafico (localizar intervalos con cambio de signo)\n"
        "  2. Biseccion\n"
        "  3. Falsa posicion\n"
        "  4. Iteracion de punto fijo\n"
        "  5. Newton-Raphson\n"
        "  6. Secante\n"
        "  7. Newton-Raphson modificado (raices multiples)\n"
        "  0. Salir\n";
    int opcion;
    while (true) {
        opcion = leerEntero("Opcion: ");
        if (opcion >= 0 && opcion <= 7) break;
        std::cout << "  -> Elige un numero entre 0 y 7.\n";
    }
    if (opcion == 0) return std::nullopt;

    static const TipoMetodo mapa[] = {TipoMetodo::GRAFICO,        TipoMetodo::BISECCION,
                                       TipoMetodo::FALSA_POSICION, TipoMetodo::PUNTO_FIJO,
                                       TipoMetodo::NEWTON_RAPHSON, TipoMetodo::SECANTE,
                                       TipoMetodo::NEWTON_RAPHSON_MODIFICADO};
    TipoMetodo tipo = mapa[opcion - 1];

    SolicitudCalculo solicitud;
    solicitud.expresionF = expresionF;
    solicitud.tipo = tipo;
    ParametrosMetodo& p = solicitud.params;

    switch (tipo) {
        case TipoMetodo::GRAFICO:
            p.a = leerDouble("Rango: x minimo = ");
            p.b = leerDouble("Rango: x maximo = ");
            break;
        case TipoMetodo::BISECCION:
        case TipoMetodo::FALSA_POSICION:
            p.a = leerDouble("Extremo a del intervalo = ");
            p.b = leerDouble("Extremo b del intervalo = ");
            p.tolerancia = leerDouble("Tolerancia (ej. 0.0001) = ");
            p.iteracionesMaximas = leerEntero("Numero maximo de iteraciones (ej. 50) = ");
            break;
        case TipoMetodo::PUNTO_FIJO:
            solicitud.expresionG = leerLinea("g(x) = (despejada de f(x) = 0, misma sintaxis) ");
            p.x0 = leerDouble("Valor inicial x0 = ");
            p.tolerancia = leerDouble("Tolerancia (ej. 0.0001) = ");
            p.iteracionesMaximas = leerEntero("Numero maximo de iteraciones (ej. 50) = ");
            break;
        case TipoMetodo::NEWTON_RAPHSON:
            p.x0 = leerDouble("Valor inicial x0 = ");
            p.tolerancia = leerDouble("Tolerancia (ej. 0.0001) = ");
            p.iteracionesMaximas = leerEntero("Numero maximo de iteraciones (ej. 50) = ");
            break;
        case TipoMetodo::SECANTE:
            p.x0 = leerDouble("Valor inicial x0 = ");
            p.x1 = leerDouble("Valor inicial x1 = ");
            p.tolerancia = leerDouble("Tolerancia (ej. 0.0001) = ");
            p.iteracionesMaximas = leerEntero("Numero maximo de iteraciones (ej. 50) = ");
            break;
        case TipoMetodo::NEWTON_RAPHSON_MODIFICADO:
            p.x0 = leerDouble("Valor inicial x0 = ");
            if (leerSiNo("¿Conoces la multiplicidad de la raiz? (s/n): ")) {
                p.multiplicidad = leerEntero("Multiplicidad m (entero positivo) = ");
            }
            p.tolerancia = leerDouble("Tolerancia (ej. 0.0001) = ");
            p.iteracionesMaximas = leerEntero("Numero maximo de iteraciones (ej. 50) = ");
            break;
    }

    return solicitud;
}

void InterfazConsola::mostrarTabla(const RespuestaCalculo& respuesta) const {
    const int ancho = 14;
    std::cout << "\nTabla de iteraciones:\n";
    std::cout << std::setw(ancho) << respuesta.columnas[0];
    for (size_t j = 1; j < respuesta.columnas.size(); ++j) std::cout << std::setw(ancho) << respuesta.columnas[j];
    std::cout << "\n";

    std::cout << std::fixed << std::setprecision(6);
    for (const auto& fila : respuesta.tabla) {
        std::cout << std::setw(ancho) << fila.numero;
        for (const auto& [nombre, valor] : fila.valores) {
            (void)nombre;
            std::cout << std::setw(ancho) << valor;
        }
        std::cout << "\n";
    }
    if (respuesta.tabla.empty()) {
        std::cout << "  (sin iteraciones: la raíz se encontró de forma exacta o inmediata)\n";
    }
}

void InterfazConsola::ofrecerGrafica(const std::string& expresionF, const ResultadoFinal& resultado,
                                      const ParametrosMetodo& params) {
    if (!leerSiNo("\n¿Deseas ver la grafica de f(x) integrada en esta ventana? (s/n): ")) return;

    double xMin, xMax;
    if (params.a.has_value() && params.b.has_value()) {
        double margen = std::max(1.0, (*params.b - *params.a) * 0.3);
        xMin = *params.a - margen;
        xMax = *params.b + margen;
    } else if (resultado.exito) {
        xMin = resultado.raiz - 10.0;
        xMax = resultado.raiz + 10.0;
    } else {
        xMin = -10.0;
        xMax = 10.0;
    }

    const double xMinSugerido = xMin;
    const double xMaxSugerido = xMax;
    std::cout << "Rango sugerido: [" << xMin << ", " << xMax << "]. Presiona Enter para aceptarlo"
              << " o escribe otro valor.\n";
    std::string linea = leerLinea("x minimo [" + std::to_string(xMin) + "]: ");
    if (!aMinusculasRecortado(linea).empty()) {
        try { xMin = std::stod(linea); } catch (...) {}
    }
    linea = leerLinea("x maximo [" + std::to_string(xMax) + "]: ");
    if (!aMinusculasRecortado(linea).empty()) {
        try { xMax = std::stod(linea); } catch (...) {}
    }
    if (!(xMin < xMax)) {
        std::cout << "  -> Rango invalido (x minimo debe ser menor que x maximo); se usa el rango sugerido.\n";
        xMin = xMinSugerido;
        xMax = xMaxSugerido;
    }

    std::optional<double> raiz = resultado.exito ? std::optional<double>(resultado.raiz) : std::nullopt;
    std::cout << "\n" << controlador_.generarGrafica(expresionF, xMin, xMax, raiz) << "\n";
}

void InterfazConsola::mostrarRespuesta(const RespuestaCalculo& respuesta, const std::string& expresionF) {
    std::cout << "\n----------------------------------------------------------------\n";
    if (!respuesta.exito) {
        std::cout << "No fue posible realizar el calculo.\n";
        std::cout << "Motivo: " << respuesta.mensajeError << "\n";
        std::cout << "Corrige el dato indicado e intenta de nuevo.\n";
        (void)expresionF;
        return;
    }

    std::cout << "Metodo: " << respuesta.nombreMetodo << "\n";
    mostrarTabla(respuesta);

    std::cout << "\nResultado final:\n";
    std::cout << "  Exito:            " << (respuesta.resultado.exito ? "si" : "no") << "\n";
    std::cout << "  Raiz aproximada:  " << respuesta.resultado.raiz << "\n";
    std::cout << "  Iteraciones:      " << respuesta.resultado.iteraciones << "\n";
    std::cout << "  Error final:      " << respuesta.resultado.errorFinal << "\n";
    std::cout << "  Criterio de paro: " << respuesta.resultado.criterioParoAlcanzado << "\n";
    std::cout << "  Mensaje:          " << respuesta.resultado.mensaje << "\n";
}

void InterfazConsola::ejecutar() {
    try {
        mostrarBienvenida();
        while (true) {
            auto solicitudOpt = capturarSolicitud();
            if (!solicitudOpt) break;

            RespuestaCalculo respuesta = controlador_.ejecutarCalculo(*solicitudOpt);
            mostrarRespuesta(respuesta, solicitudOpt->expresionF);
            if (respuesta.exito) {
                ofrecerGrafica(solicitudOpt->expresionF, respuesta.resultado, solicitudOpt->params);
            }

            std::cout << "\n";
            if (!leerSiNo("¿Deseas realizar otro calculo? (s/n): ")) break;
            std::cout << "\n";
        }
    } catch (const SalirAplicacion&) {
        // Salida solicitada por el usuario (EOF o 'salir'): termina sin error.
    }
    std::cout << "\nGracias por usar la aplicacion. Hasta luego.\n";
}

} // namespace mn::ui
