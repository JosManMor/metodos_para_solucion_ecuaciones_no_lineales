#pragma once

#include <memory>
#include <stdexcept>
#include <string>

namespace mn::core {

// Se lanza cuando la expresión f(x) no puede interpretarse (sintaxis inválida).
class ErrorSintaxis : public std::runtime_error {
public:
    explicit ErrorSintaxis(const std::string& mensaje) : std::runtime_error(mensaje) {}
};

// Se lanza cuando, al evaluar f(x) en un punto concreto, el resultado queda
// fuera del dominio matemático (log de negativo, división entre cero, etc.).
class ErrorDominio : public std::runtime_error {
public:
    explicit ErrorDominio(const std::string& mensaje) : std::runtime_error(mensaje) {}
};

namespace detalle { class NodoExpr; }

// Representa una función matemática f(x) ingresada como texto por el usuario.
// Admite: + - * / ^, paréntesis, constantes pi/e, y las funciones
// sin cos tan asin acos atan sinh cosh tanh exp ln log sqrt abs.
// La derivada se aproxima numéricamente (diferencias centradas), por lo que
// el usuario nunca necesita escribir f'(x) a mano.
class Funcion {
public:
    explicit Funcion(const std::string& expresion);
    ~Funcion();
    Funcion(const Funcion&);
    Funcion& operator=(const Funcion&);
    Funcion(Funcion&&) noexcept;
    Funcion& operator=(Funcion&&) noexcept;

    double evaluar(double x) const;
    double derivada(double x, double h = 1e-5) const;
    double segundaDerivada(double x, double h = 1e-4) const;

    const std::string& expresionOriginal() const { return expresion_; }

private:
    std::string expresion_;
    std::shared_ptr<detalle::NodoExpr> raiz_;
};

} // namespace mn::core
