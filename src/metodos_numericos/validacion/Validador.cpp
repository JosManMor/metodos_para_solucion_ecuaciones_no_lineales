#include "metodos_numericos/validacion/Validador.hpp"

#include <cmath>

#include "metodos_numericos/validacion/ErrorValidacion.hpp"

namespace mn::validacion {

using mn::core::ErrorDominio;
using mn::core::Funcion;
using mn::core::ParametrosMetodo;
using mn::metodos::TipoMetodo;

void Validador::validarComunes(const ParametrosMetodo& p) {
    if (p.tolerancia <= 0.0) {
        throw ErrorValidacion("La tolerancia debe ser un número positivo mayor que cero.");
    }
    if (p.iteracionesMaximas <= 0) {
        throw ErrorValidacion("El número máximo de iteraciones debe ser un entero positivo.");
    }
}

void Validador::validarIntervalo(const Funcion& f, const ParametrosMetodo& p) {
    if (!p.a.has_value() || !p.b.has_value()) {
        throw ErrorValidacion("Este método requiere los extremos del intervalo [a, b].");
    }
    double a = *p.a;
    double b = *p.b;
    if (!(a < b)) {
        throw ErrorValidacion("El intervalo es inválido: se requiere a < b.");
    }
    double fa, fb;
    try {
        fa = f.evaluar(a);
    } catch (const ErrorDominio& e) {
        throw ErrorValidacion(std::string("f(a) no se puede evaluar: ") + e.what());
    }
    try {
        fb = f.evaluar(b);
    } catch (const ErrorDominio& e) {
        throw ErrorValidacion(std::string("f(b) no se puede evaluar: ") + e.what());
    }
    if (fa == 0.0 || fb == 0.0) {
        return; // una de las fronteras ya es raíz exacta; el método la detectará de inmediato
    }
    if (fa * fb > 0.0) {
        throw ErrorValidacion(
            "El intervalo [a, b] no contiene un cambio de signo (f(a) y f(b) tienen el mismo signo). "
            "Elige otro intervalo o usa el método gráfico para localizar uno válido.");
    }
}

void Validador::validarPuntoFijo(const Funcion& g, const ParametrosMetodo& p) {
    if (!p.x0.has_value()) {
        throw ErrorValidacion("El método de punto fijo requiere un valor inicial x0.");
    }
    try {
        g.evaluar(*p.x0);
    } catch (const ErrorDominio& e) {
        throw ErrorValidacion(std::string("g(x0) no se puede evaluar: ") + e.what());
    }
}

void Validador::validarNewtonRaphson(const Funcion& f, const ParametrosMetodo& p) {
    if (!p.x0.has_value()) {
        throw ErrorValidacion("El método de Newton-Raphson requiere un valor inicial x0.");
    }
    double fx0;
    try {
        fx0 = f.evaluar(*p.x0);
        (void)fx0;
    } catch (const ErrorDominio& e) {
        throw ErrorValidacion(std::string("f(x0) no se puede evaluar: ") + e.what());
    }
    double derivada;
    try {
        derivada = f.derivada(*p.x0);
    } catch (const ErrorDominio& e) {
        throw ErrorValidacion(std::string("f'(x0) no se puede evaluar: ") + e.what());
    }
    if (std::fabs(derivada) < 1e-12) {
        throw ErrorValidacion("f'(x0) es prácticamente cero: Newton-Raphson no puede iniciar desde este punto.");
    }
}

void Validador::validarSecante(const ParametrosMetodo& p) {
    if (!p.x0.has_value() || !p.x1.has_value()) {
        throw ErrorValidacion("El método de la secante requiere dos valores iniciales x0 y x1.");
    }
    if (std::fabs(*p.x1 - *p.x0) < 1e-14) {
        throw ErrorValidacion("x0 y x1 deben ser diferentes para el método de la secante.");
    }
}

void Validador::validarNewtonModificado(const Funcion& f, const ParametrosMetodo& p) {
    validarNewtonRaphson(f, p);
    if (p.multiplicidad.has_value() && *p.multiplicidad <= 0) {
        throw ErrorValidacion("La multiplicidad declarada debe ser un entero positivo.");
    }
}

void Validador::validarGrafico(const ParametrosMetodo& p) {
    if (!p.a.has_value() || !p.b.has_value()) {
        throw ErrorValidacion("El método gráfico requiere un rango [x_min, x_max].");
    }
    if (!(*p.a < *p.b)) {
        throw ErrorValidacion("El rango del método gráfico es inválido: se requiere x_min < x_max.");
    }
}

void Validador::validar(TipoMetodo tipo, const Funcion& f, const ParametrosMetodo& p) {
    if (tipo != TipoMetodo::GRAFICO) {
        validarComunes(p);
    }
    switch (tipo) {
        case TipoMetodo::GRAFICO:
            validarGrafico(p);
            break;
        case TipoMetodo::BISECCION:
        case TipoMetodo::FALSA_POSICION:
            validarIntervalo(f, p);
            break;
        case TipoMetodo::PUNTO_FIJO:
            validarPuntoFijo(f, p);
            break;
        case TipoMetodo::NEWTON_RAPHSON:
            validarNewtonRaphson(f, p);
            break;
        case TipoMetodo::SECANTE:
            validarSecante(p);
            break;
        case TipoMetodo::NEWTON_RAPHSON_MODIFICADO:
            validarNewtonModificado(f, p);
            break;
    }
}

} // namespace mn::validacion
