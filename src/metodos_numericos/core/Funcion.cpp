#include "metodos_numericos/core/Funcion.hpp"

#include <cctype>
#include <cmath>
#include <memory>
#include <vector>

namespace mn::core {

// ------------------------------------------------------------------------
// Árbol de sintaxis abstracta (AST) para expresiones f(x).
// Todo el parser vive en este .cpp: el resto del sistema solo conoce Funcion.
// ------------------------------------------------------------------------
namespace detalle {

class NodoExpr {
public:
    virtual ~NodoExpr() = default;
    virtual double evaluar(double x) const = 0;
};

namespace {

double verificarFinito(double valor, const std::string& contexto) {
    if (std::isnan(valor) || std::isinf(valor)) {
        throw ErrorDominio("La expresión produce un valor no numérico en " + contexto +
                           " (verifica el dominio de la función).");
    }
    return valor;
}

class NodoNumero : public NodoExpr {
public:
    explicit NodoNumero(double valor) : valor_(valor) {}
    double evaluar(double) const override { return valor_; }

private:
    double valor_;
};

class NodoVariable : public NodoExpr {
public:
    double evaluar(double x) const override { return x; }
};

class NodoUnario : public NodoExpr {
public:
    NodoUnario(char op, std::shared_ptr<NodoExpr> hijo) : op_(op), hijo_(std::move(hijo)) {}
    double evaluar(double x) const override {
        double v = hijo_->evaluar(x);
        return op_ == '-' ? -v : v;
    }

private:
    char op_;
    std::shared_ptr<NodoExpr> hijo_;
};

class NodoBinario : public NodoExpr {
public:
    NodoBinario(char op, std::shared_ptr<NodoExpr> izq, std::shared_ptr<NodoExpr> der)
        : op_(op), izq_(std::move(izq)), der_(std::move(der)) {}

    double evaluar(double x) const override {
        double a = izq_->evaluar(x);
        double b = der_->evaluar(x);
        switch (op_) {
            case '+': return verificarFinito(a + b, "una suma");
            case '-': return verificarFinito(a - b, "una resta");
            case '*': return verificarFinito(a * b, "una multiplicación");
            case '/':
                if (std::fabs(b) < 1e-300) {
                    throw ErrorDominio("División entre cero al evaluar f(x).");
                }
                return verificarFinito(a / b, "una división");
            case '^': {
                if (a < 0.0 && std::fabs(b - std::round(b)) > 1e-9) {
                    throw ErrorDominio("Base negativa con exponente no entero (resultado complejo no soportado).");
                }
                return verificarFinito(std::pow(a, b), "una potencia");
            }
            default:
                throw ErrorSintaxis("Operador no reconocido.");
        }
    }

private:
    char op_;
    std::shared_ptr<NodoExpr> izq_;
    std::shared_ptr<NodoExpr> der_;
};

class NodoFuncion : public NodoExpr {
public:
    NodoFuncion(std::string nombre, std::shared_ptr<NodoExpr> arg)
        : nombre_(std::move(nombre)), arg_(std::move(arg)) {}

    double evaluar(double x) const override {
        double v = arg_->evaluar(x);
        if (nombre_ == "sin") return std::sin(v);
        if (nombre_ == "cos") return std::cos(v);
        if (nombre_ == "tan") return std::tan(v);
        if (nombre_ == "asin") {
            if (v < -1.0 || v > 1.0) throw ErrorDominio("asin(x) requiere -1 <= x <= 1.");
            return std::asin(v);
        }
        if (nombre_ == "acos") {
            if (v < -1.0 || v > 1.0) throw ErrorDominio("acos(x) requiere -1 <= x <= 1.");
            return std::acos(v);
        }
        if (nombre_ == "atan") return std::atan(v);
        if (nombre_ == "sinh") return std::sinh(v);
        if (nombre_ == "cosh") return std::cosh(v);
        if (nombre_ == "tanh") return std::tanh(v);
        if (nombre_ == "exp") return verificarFinito(std::exp(v), "exp(x)");
        if (nombre_ == "ln") {
            if (v <= 0.0) throw ErrorDominio("ln(x) requiere x > 0.");
            return std::log(v);
        }
        if (nombre_ == "log") {
            if (v <= 0.0) throw ErrorDominio("log(x) requiere x > 0.");
            return std::log10(v);
        }
        if (nombre_ == "sqrt") {
            if (v < 0.0) throw ErrorDominio("sqrt(x) requiere x >= 0.");
            return std::sqrt(v);
        }
        if (nombre_ == "abs") return std::fabs(v);
        throw ErrorSintaxis("Función no reconocida: " + nombre_ + "(...)");
    }

private:
    std::string nombre_;
    std::shared_ptr<NodoExpr> arg_;
};

// ------------------------- Tokenizador y parser -------------------------

enum class TipoToken { NUMERO, IDENT, MAS, MENOS, MUL, DIV, POT, PARI, PARD, FIN };

struct Token {
    TipoToken tipo;
    std::string texto;
    double numero = 0.0;
};

std::vector<Token> tokenizar(const std::string& entrada) {
    std::vector<Token> tokens;
    size_t i = 0;
    const size_t n = entrada.size();
    while (i < n) {
        char c = entrada[i];
        if (std::isspace(static_cast<unsigned char>(c))) { ++i; continue; }
        if (std::isdigit(static_cast<unsigned char>(c)) || c == '.') {
            size_t j = i;
            bool puntoVisto = false;
            while (j < n && (std::isdigit(static_cast<unsigned char>(entrada[j])) ||
                              (entrada[j] == '.' && !puntoVisto))) {
                if (entrada[j] == '.') puntoVisto = true;
                ++j;
            }
            // notación científica opcional: 1e-3, 2.5e10
            if (j < n && (entrada[j] == 'e' || entrada[j] == 'E') &&
                (j + 1 < n) &&
                (std::isdigit(static_cast<unsigned char>(entrada[j + 1])) ||
                 ((entrada[j + 1] == '+' || entrada[j + 1] == '-') && j + 2 < n &&
                  std::isdigit(static_cast<unsigned char>(entrada[j + 2]))))) {
                ++j;
                if (entrada[j] == '+' || entrada[j] == '-') ++j;
                while (j < n && std::isdigit(static_cast<unsigned char>(entrada[j]))) ++j;
            }
            Token t;
            t.tipo = TipoToken::NUMERO;
            t.texto = entrada.substr(i, j - i);
            t.numero = std::stod(t.texto);
            tokens.push_back(t);
            i = j;
            continue;
        }
        if (std::isalpha(static_cast<unsigned char>(c)) || c == '_') {
            size_t j = i;
            while (j < n && (std::isalnum(static_cast<unsigned char>(entrada[j])) || entrada[j] == '_')) ++j;
            Token t;
            t.tipo = TipoToken::IDENT;
            t.texto = entrada.substr(i, j - i);
            tokens.push_back(t);
            i = j;
            continue;
        }
        switch (c) {
            case '+': tokens.push_back({TipoToken::MAS, "+"}); break;
            case '-': tokens.push_back({TipoToken::MENOS, "-"}); break;
            case '*': tokens.push_back({TipoToken::MUL, "*"}); break;
            case '/': tokens.push_back({TipoToken::DIV, "/"}); break;
            case '^': tokens.push_back({TipoToken::POT, "^"}); break;
            case '(': tokens.push_back({TipoToken::PARI, "("}); break;
            case ')': tokens.push_back({TipoToken::PARD, ")"}); break;
            default:
                throw ErrorSintaxis(std::string("Carácter no reconocido en la expresión: '") + c + "'.");
        }
        ++i;
    }
    tokens.push_back({TipoToken::FIN, ""});
    return tokens;
}

class Parser {
public:
    explicit Parser(std::vector<Token> tokens) : tokens_(std::move(tokens)) {}

    std::shared_ptr<NodoExpr> analizar() {
        auto nodo = expresion();
        esperar(TipoToken::FIN, "se esperaba el fin de la expresión");
        return nodo;
    }

private:
    std::vector<Token> tokens_;
    size_t pos_ = 0;

    const Token& actual() const { return tokens_[pos_]; }

    bool coincide(TipoToken t) const { return actual().tipo == t; }

    Token avanzar() { return tokens_[pos_++]; }

    void esperar(TipoToken t, const std::string& mensaje) {
        if (!coincide(t)) {
            throw ErrorSintaxis("Error de sintaxis en la expresión: " + mensaje + ".");
        }
        avanzar();
    }

    // expresion := termino (('+'|'-') termino)*
    std::shared_ptr<NodoExpr> expresion() {
        auto izq = termino();
        while (coincide(TipoToken::MAS) || coincide(TipoToken::MENOS)) {
            char op = avanzar().texto[0];
            auto der = termino();
            izq = std::make_shared<NodoBinario>(op, izq, der);
        }
        return izq;
    }

    // termino := factor (('*'|'/') factor)*
    std::shared_ptr<NodoExpr> termino() {
        auto izq = factorUnario();
        while (coincide(TipoToken::MUL) || coincide(TipoToken::DIV)) {
            char op = avanzar().texto[0];
            auto der = factorUnario();
            izq = std::make_shared<NodoBinario>(op, izq, der);
        }
        return izq;
    }

    // factorUnario := ('-'|'+') factorUnario | potencia
    std::shared_ptr<NodoExpr> factorUnario() {
        if (coincide(TipoToken::MENOS) || coincide(TipoToken::MAS)) {
            char op = avanzar().texto[0];
            auto hijo = factorUnario();
            if (op == '-') return std::make_shared<NodoUnario>('-', hijo);
            return hijo;
        }
        return potencia();
    }

    // potencia := primario ('^' factorUnario)?   (asociativo a la derecha)
    std::shared_ptr<NodoExpr> potencia() {
        auto base = primario();
        if (coincide(TipoToken::POT)) {
            avanzar();
            auto exponente = factorUnario();
            return std::make_shared<NodoBinario>('^', base, exponente);
        }
        return base;
    }

    // primario := NUMERO | IDENT ['(' expresion ')'] | '(' expresion ')'
    std::shared_ptr<NodoExpr> primario() {
        if (coincide(TipoToken::NUMERO)) {
            double v = avanzar().numero;
            return std::make_shared<NodoNumero>(v);
        }
        if (coincide(TipoToken::IDENT)) {
            std::string nombre = avanzar().texto;
            if (coincide(TipoToken::PARI)) {
                avanzar();
                auto arg = expresion();
                esperar(TipoToken::PARD, "falta un paréntesis de cierre ')'");
                return std::make_shared<NodoFuncion>(nombre, arg);
            }
            if (nombre == "x") return std::make_shared<NodoVariable>();
            if (nombre == "pi") return std::make_shared<NodoNumero>(M_PI);
            if (nombre == "e") return std::make_shared<NodoNumero>(M_E);
            throw ErrorSintaxis("Identificador no reconocido: '" + nombre +
                                "' (variable válida: x; constantes: pi, e).");
        }
        if (coincide(TipoToken::PARI)) {
            avanzar();
            auto nodo = expresion();
            esperar(TipoToken::PARD, "falta un paréntesis de cierre ')'");
            return nodo;
        }
        throw ErrorSintaxis("Se esperaba un número, 'x' o una función en la expresión.");
    }
};

} // namespace (anónimo)
} // namespace detalle

// ------------------------------------------------------------------------
// Implementación de Funcion
// ------------------------------------------------------------------------

Funcion::Funcion(const std::string& expresion) : expresion_(expresion) {
    if (expresion_.empty()) {
        throw ErrorSintaxis("La expresión de f(x) no puede estar vacía.");
    }
    auto tokens = detalle::tokenizar(expresion_);
    detalle::Parser parser(std::move(tokens));
    raiz_ = parser.analizar();
}

Funcion::~Funcion() = default;
Funcion::Funcion(const Funcion&) = default;
Funcion& Funcion::operator=(const Funcion&) = default;
Funcion::Funcion(Funcion&&) noexcept = default;
Funcion& Funcion::operator=(Funcion&&) noexcept = default;

double Funcion::evaluar(double x) const {
    return raiz_->evaluar(x);
}

double Funcion::derivada(double x, double h) const {
    double adelante = raiz_->evaluar(x + h);
    double atras = raiz_->evaluar(x - h);
    return (adelante - atras) / (2.0 * h);
}

double Funcion::segundaDerivada(double x, double h) const {
    double adelante = raiz_->evaluar(x + h);
    double centro = raiz_->evaluar(x);
    double atras = raiz_->evaluar(x - h);
    return (adelante - 2.0 * centro + atras) / (h * h);
}

} // namespace mn::core
