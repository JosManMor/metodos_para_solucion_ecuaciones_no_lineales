// Pruebas de verificación de los métodos numéricos, sin ningún framework
// externo (no hay cmake/pkg-config disponibles en este entorno). Cada caso
// compara la raíz obtenida contra un valor de referencia conocido.
#include <cmath>
#include <iostream>
#include <string>

#include "metodos_numericos/core/Funcion.hpp"
#include "metodos_numericos/metodos/Biseccion.hpp"
#include "metodos_numericos/metodos/FalsaPosicion.hpp"
#include "metodos_numericos/metodos/MetodoGrafico.hpp"
#include "metodos_numericos/metodos/NewtonRaphson.hpp"
#include "metodos_numericos/metodos/NewtonRaphsonModificado.hpp"
#include "metodos_numericos/metodos/PuntoFijo.hpp"
#include "metodos_numericos/metodos/Secante.hpp"
#include "metodos_numericos/validacion/ErrorValidacion.hpp"
#include "metodos_numericos/validacion/Validador.hpp"

using namespace mn::core;
using namespace mn::metodos;
using namespace mn::validacion;

namespace {

int fallos = 0;
int totales = 0;

void verificar(const std::string& caso, bool condicion, const std::string& detalle = "") {
    ++totales;
    if (condicion) {
        std::cout << "[OK]   " << caso << "\n";
    } else {
        ++fallos;
        std::cout << "[FAIL] " << caso << (detalle.empty() ? "" : (" -> " + detalle)) << "\n";
    }
}

void cercaDe(const std::string& caso, double obtenido, double esperado, double tol = 1e-3) {
    verificar(caso, std::fabs(obtenido - esperado) < tol,
              "obtenido=" + std::to_string(obtenido) + " esperado=" + std::to_string(esperado));
}

} // namespace

int main() {
    // --- Bisección: x^3 - 4x - 9 = 0, raíz real ~2.7065 ---
    {
        Funcion f("x^3 - 4*x - 9");
        ParametrosMetodo p;
        p.a = 2.0; p.b = 3.0; p.tolerancia = 1e-6; p.iteracionesMaximas = 100;
        Biseccion m(f, p);
        auto r = m.ejecutar();
        cercaDe("Bisección x^3-4x-9 en [2,3]", r.raiz, 2.706525);
    }

    // --- Falsa posición: misma función ---
    {
        Funcion f("x^3 - 4*x - 9");
        ParametrosMetodo p;
        p.a = 2.0; p.b = 3.0; p.tolerancia = 1e-6; p.iteracionesMaximas = 100;
        FalsaPosicion m(f, p);
        auto r = m.ejecutar();
        cercaDe("Falsa posición x^3-4x-9 en [2,3]", r.raiz, 2.706525);
    }

    // --- Punto fijo: x = (4x+9)^(1/3), misma raíz ---
    {
        Funcion g("(4*x+9)^(1/3)");
        ParametrosMetodo p;
        p.x0 = 2.0; p.tolerancia = 1e-6; p.iteracionesMaximas = 100;
        PuntoFijo m(g, p);
        auto r = m.ejecutar();
        cercaDe("Punto fijo (4x+9)^(1/3)", r.raiz, 2.706525);
    }

    // --- Newton-Raphson: 2e^x - 5 = 0 -> x = ln(2.5) ---
    {
        Funcion f("2*exp(x) - 5");
        ParametrosMetodo p;
        p.x0 = 1.0; p.tolerancia = 1e-8; p.iteracionesMaximas = 100;
        NewtonRaphson m(f, p);
        auto r = m.ejecutar();
        cercaDe("Newton-Raphson 2e^x-5", r.raiz, std::log(2.5), 1e-5);
    }

    // --- Secante: sin(x) - 0.5x = 0, raíz no trivial ~1.8955 ---
    {
        Funcion f("sin(x) - 0.5*x");
        ParametrosMetodo p;
        p.x0 = 1.0; p.x1 = 2.0; p.tolerancia = 1e-6; p.iteracionesMaximas = 100;
        Secante m(f, p);
        auto r = m.ejecutar();
        cercaDe("Secante sin(x)-0.5x", r.raiz, 1.895494, 1e-3);
    }

    // --- Newton-Raphson modificado: raíz múltiple (x-2)^2 (x+1), m=2 ---
    {
        Funcion f("(x-2)^2*(x+1)");
        ParametrosMetodo p;
        p.x0 = 1.5; p.multiplicidad = 2; p.tolerancia = 1e-8; p.iteracionesMaximas = 100;
        NewtonRaphsonModificado m(f, p);
        auto r = m.ejecutar();
        cercaDe("Newton-Raphson modificado raíz doble x=2", r.raiz, 2.0, 1e-4);
        verificar("Newton modificado converge en pocas iteraciones (<10)", r.iteraciones < 10);
    }

    // --- Método gráfico: debe detectar un cambio de signo cerca de x=2.7 ---
    {
        Funcion f("x^3 - 4*x - 9");
        ParametrosMetodo p;
        p.a = -5.0; p.b = 5.0;
        MetodoGrafico m(f, p);
        auto r = m.ejecutar();
        verificar("Método gráfico detecta al menos un cambio de signo", r.exito);
    }

    // --- Validación: intervalo sin cambio de signo debe rechazarse ---
    {
        Funcion f("x^2 + 4");
        ParametrosMetodo p;
        p.a = -1.0; p.b = 1.0; p.tolerancia = 1e-6; p.iteracionesMaximas = 50;
        bool lanzo = false;
        try {
            Validador::validar(TipoMetodo::BISECCION, f, p);
        } catch (const ErrorValidacion&) {
            lanzo = true;
        }
        verificar("Validador rechaza intervalo sin cambio de signo", lanzo);
    }

    // --- Validación: tolerancia negativa debe rechazarse ---
    {
        Funcion f("x^2 - 4");
        ParametrosMetodo p;
        p.a = 0.0; p.b = 3.0; p.tolerancia = -0.1; p.iteracionesMaximas = 50;
        bool lanzo = false;
        try {
            Validador::validar(TipoMetodo::BISECCION, f, p);
        } catch (const ErrorValidacion&) {
            lanzo = true;
        }
        verificar("Validador rechaza tolerancia negativa", lanzo);
    }

    // --- Dominio: ln(x) con x <= 0 debe lanzar ErrorDominio al evaluar ---
    {
        Funcion f("ln(x)");
        bool lanzo = false;
        try {
            f.evaluar(-1.0);
        } catch (const ErrorDominio&) {
            lanzo = true;
        }
        verificar("ln(x) con x negativo lanza ErrorDominio", lanzo);
    }

    // --- Sintaxis: expresión mal formada debe lanzar ErrorSintaxis ---
    {
        bool lanzo = false;
        try {
            Funcion f("x^3 -- 4*x )");
            (void)f;
        } catch (const ErrorSintaxis&) {
            lanzo = true;
        }
        verificar("Expresión mal formada lanza ErrorSintaxis", lanzo);
    }

    std::cout << "\n" << (totales - fallos) << "/" << totales << " pruebas superadas.\n";
    return fallos == 0 ? 0 : 1;
}
