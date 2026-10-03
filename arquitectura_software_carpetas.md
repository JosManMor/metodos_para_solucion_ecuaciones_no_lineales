# Arquitectura de Software y Estructura de Carpetas
## Aplicación de Métodos Numéricos — Unidad 2

**Estudiante:** Carrillo Valencia Ruth
**Grupo:** AM3B — Ingeniería Mecatrónica
**Lenguaje:** C++

Este documento complementa el diseño del sistema (`diseño_sistema.md`) y define, de forma concreta, la **arquitectura de software** adoptada y la **organización de carpetas y archivos** del proyecto en C++, de modo que cada módulo descrito en el diseño tenga un lugar físico dentro del repositorio.

---

## 1. Estilo arquitectónico adoptado

Se adopta una **arquitectura en capas con inyección de dependencias ligera**, equivalente a un patrón Modelo–Vista–Controlador (MVC) adaptado a una aplicación de escritorio en C++:

```
Vista (UI) ──► Controlador ──► Validación ──► Lógica numérica (Modelo)
                    │                               │
                    └────────────► Graficación ◄────┘
```

- **Vista (UI):** formularios, tabla de iteraciones, lienzo de gráfica, mensajes de error.
- **Controlador:** orquesta el flujo de un cálculo y el ciclo de "repetir sin cerrar la aplicación".
- **Validación:** reglas de entrada y detección de condiciones numéricas inválidas.
- **Modelo (lógica numérica):** los seis métodos + Newton-Raphson modificado, todos bajo una interfaz común.
- **Graficación:** módulo independiente que dibuja f(x) y la raíz dentro de la misma ventana.

Reglas de dependencia (para mantener el bajo acoplamiento):

1. La capa de **Vista** solo conoce al **Controlador** (nunca llama directamente a un método numérico o al validador).
2. El **Controlador** conoce a **Validación**, **Modelo** y **Graficación**, pero estas capas no conocen al Controlador ni a la Vista (evita dependencias circulares).
3. El **Modelo** (métodos numéricos) no depende de la UI ni de la graficación; solo recibe `Funcion` y `ParametrosMetodo` y devuelve resultados.
4. La **Graficación** depende únicamente de `Funcion` y de los resultados del cálculo, nunca de la UI.

## 2. Mapeo de capas a módulos de código

| Capa | Módulo (carpeta) | Clases principales |
|---|---|---|
| Vista | `ui/` | VentanaPrincipal, PanelParametros, PanelResultados, PanelGrafica |
| Controlador | `control/` | ControladorApp |
| Validación | `validacion/` | Validador, GestorErrores |
| Modelo (núcleo de datos) | `core/` | Funcion, ParametrosMetodo, IteracionResultado, ResultadoFinal, ErrorValidacion |
| Modelo (métodos numéricos) | `metodos/` | MetodoNumerico (interfaz), Biseccion, FalsaPosicion, PuntoFijo, NewtonRaphson, Secante, NewtonRaphsonModificado, MetodoGrafico, FabricaDeMetodos |
| Graficación | `grafica/` | MotorGrafico, DatosGrafica |

## 3. Estructura de carpetas del proyecto

```
proyecto-metodos-numericos/
├── CMakeLists.txt                     # Configuración de compilación (único binario final)
├── README.md                          # Instrucciones de compilación y ejecución
│
├── docs/                               # Documentación del proyecto
│   ├── diseño_sistema.md
│   ├── arquitectura_software_carpetas.md
│   ├── informe_tecnico.md
│   └── manual_usuario.md
│
├── recursos/                           # Recursos estáticos embebidos en la app
│   ├── iconos/
│   └── estilos/
│
├── include/                            # Encabezados (.hpp) — contrato público de cada clase
│   └── metodos_numericos/
│       ├── core/
│       │   ├── Funcion.hpp
│       │   ├── ParametrosMetodo.hpp
│       │   ├── IteracionResultado.hpp
│       │   ├── ResultadoFinal.hpp
│       │   └── ErrorValidacion.hpp
│       │
│       ├── metodos/
│       │   ├── MetodoNumerico.hpp          # interfaz común (clase base abstracta)
│       │   ├── MetodoGrafico.hpp
│       │   ├── Biseccion.hpp
│       │   ├── FalsaPosicion.hpp
│       │   ├── PuntoFijo.hpp
│       │   ├── NewtonRaphson.hpp
│       │   ├── Secante.hpp
│       │   ├── NewtonRaphsonModificado.hpp
│       │   └── FabricaDeMetodos.hpp
│       │
│       ├── validacion/
│       │   ├── Validador.hpp
│       │   └── GestorErrores.hpp
│       │
│       ├── grafica/
│       │   ├── DatosGrafica.hpp
│       │   └── MotorGrafico.hpp
│       │
│       ├── control/
│       │   └── ControladorApp.hpp
│       │
│       └── ui/
│           ├── VentanaPrincipal.hpp
│           ├── PanelParametros.hpp
│           ├── PanelResultados.hpp
│           └── PanelGrafica.hpp
│
├── src/                                 # Implementación (.cpp) — misma jerarquía que include/
│   ├── main.cpp                         # Punto de entrada de la aplicación
│   └── metodos_numericos/
│       ├── core/        (Funcion.cpp, ParametrosMetodo.cpp, ...)
│       ├── metodos/      (Biseccion.cpp, FalsaPosicion.cpp, PuntoFijo.cpp,
│       │                  NewtonRaphson.cpp, Secante.cpp,
│       │                  NewtonRaphsonModificado.cpp, MetodoGrafico.cpp,
│       │                  FabricaDeMetodos.cpp)
│       ├── validacion/   (Validador.cpp, GestorErrores.cpp)
│       ├── grafica/      (MotorGrafico.cpp)
│       ├── control/      (ControladorApp.cpp)
│       └── ui/            (VentanaPrincipal.cpp, PanelParametros.cpp,
│                            PanelResultados.cpp, PanelGrafica.cpp)
│
├── tests/                               # Pruebas unitarias por método y por validación
│   ├── test_biseccion.cpp
│   ├── test_falsa_posicion.cpp
│   ├── test_punto_fijo.cpp
│   ├── test_newton_raphson.cpp
│   ├── test_secante.cpp
│   ├── test_newton_raphson_modificado.cpp
│   ├── test_validador.cpp
│   ├── test_funcion_dominio.cpp
│   └── funciones_prueba/                # Casos de prueba documentados (ver sección 4)
│       └── casos_prueba.md
│
├── evidencias/                          # Capturas/resultados exigidos como entregable
│   ├── polinomiales/
│   ├── racionales/
│   ├── exponenciales/
│   ├── logaritmicas/
│   ├── trigonometricas/
│   └── trascendentes_y_raices_multiples/
│
└── build/                               # Carpeta generada por CMake (no se versiona)
```

## 4. Convenciones de organización

- **Separación encabezado/implementación:** cada clase tiene su `.hpp` en `include/` y su `.cpp` en `src/`, siguiendo la misma ruta relativa, lo que facilita ubicar cualquier componente del diseño dentro del código.
- **Un módulo = una carpeta = una responsabilidad:** `core`, `metodos`, `validacion`, `grafica`, `control` y `ui` corresponden exactamente a las capas definidas en el diseño del sistema; ningún archivo de `metodos/` debe incluir encabezados de `ui/`, reforzando la regla de dependencia unidireccional de la sección 1.
- **Pruebas espejo de los métodos:** por cada clase en `metodos/` existe un archivo de prueba en `tests/`, y `tests/funciones_prueba/casos_prueba.md` documenta las funciones de prueba exigidas (polinomiales, racionales, exponenciales, logarítmicas, trigonométricas, trascendentes y de raíz múltiple), referenciando la tabla de la sección 16 del diseño del sistema.
- **Evidencias trazables al informe:** la carpeta `evidencias/` está organizada por familia de función, para que el informe técnico y el video puedan referenciar directamente las capturas de cada prueba.
- **Un solo ejecutable autocontenido:** `CMakeLists.txt` compila un único binario que enlaza estáticamente (o mediante las bibliotecas del sistema) la interfaz gráfica y el motor de graficación, de modo que la aplicación no dependa de ningún programa externo para calcular ni para graficar, conforme al requisito del proyecto.
- **Documentación junto al código, no dentro de él:** el informe técnico y el manual de usuario viven en `docs/`, separados del código fuente, para mantener el repositorio ordenado y facilitar la entrega.

## 5. Relación con el documento de diseño previo

Esta estructura es la contraparte física de los componentes descritos en `diseño_sistema.md`:

- La interfaz `MetodoNumerico` (sección 8.2 del diseño) se ubica en `include/metodos_numericos/metodos/MetodoNumerico.hpp`, y cada método concreto en su propio par `.hpp/.cpp` dentro de `metodos/`.
- El `Validador` y el `GestorErrores` (sección 12 del diseño) corresponden a `validacion/`.
- El `MotorGrafico` (sección 14 del diseño) corresponde a `grafica/`.
- El `ControladorApp` (sección 10 del diseño) corresponde a `control/`.
- Las pantallas descritas en la sección 13 del diseño corresponden a las clases de `ui/`.

---

**Fin del documento de arquitectura y estructura de carpetas.**
