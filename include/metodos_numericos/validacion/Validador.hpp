#pragma once

#include "metodos_numericos/core/Funcion.hpp"
#include "metodos_numericos/core/Tipos.hpp"
#include "metodos_numericos/metodos/TipoMetodo.hpp"

namespace mn::validacion {

// Valida los parámetros de un método ANTES de ejecutarlo. Si alguna regla
// falla lanza ErrorValidacion con un mensaje comprensible para el usuario;
// si la validación es exitosa, simplemente retorna.
class Validador {
public:
    static void validarComunes(const mn::core::ParametrosMetodo& p);

    static void validarIntervalo(const mn::core::Funcion& f, const mn::core::ParametrosMetodo& p);
    static void validarPuntoFijo(const mn::core::Funcion& g, const mn::core::ParametrosMetodo& p);
    static void validarNewtonRaphson(const mn::core::Funcion& f, const mn::core::ParametrosMetodo& p);
    static void validarSecante(const mn::core::ParametrosMetodo& p);
    static void validarNewtonModificado(const mn::core::Funcion& f, const mn::core::ParametrosMetodo& p);
    static void validarGrafico(const mn::core::ParametrosMetodo& p);

    // Punto de entrada único: aplica las validaciones comunes + las
    // específicas del método indicado.
    static void validar(mn::metodos::TipoMetodo tipo,
                         const mn::core::Funcion& f,
                         const mn::core::ParametrosMetodo& p);
};

} // namespace mn::validacion
