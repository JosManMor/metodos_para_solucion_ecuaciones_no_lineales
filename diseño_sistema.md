# Diseño del Sistema de Software
## Aplicación de Métodos Numéricos para la Solución de Ecuaciones No Lineales

**Instituto Tecnológico de Colima — Métodos Numéricos**
**Trabajo Final — Unidad 2**

| Dato | Valor |
|---|---|
| Estudiante | Carrillo Valencia Ruth |
| Grupo | AM3B — Ingeniería Mecatrónica |
| Periodo | Agosto–Diciembre 2026 |
| Lenguaje asignado | C++ |
| Documento | Diseño del sistema de software (sin código) |

---

## 1. Introducción

Este documento describe el diseño del sistema de software que implementa los métodos numéricos de la Unidad 2 para la solución de ecuaciones no lineales: método gráfico, bisección, falsa posición, iteración de punto fijo, Newton-Raphson, secante y Newton-Raphson modificado para raíces múltiples. El diseño se presenta de forma descriptiva, sin código fuente, y cubre la arquitectura, los módulos, las clases, los flujos de operación, la estrategia de validación/manejo de errores y la integración de la graficación dentro de la propia aplicación.

El sistema se construirá en **C++**, por asignación del docente según el ciclo de lenguajes (posición 6 de la lista → C++).

## 2. Objetivos del diseño

- Definir una arquitectura modular que separe claramente la lógica numérica, la validación, la interfaz de usuario y la graficación.
- Permitir agregar o modificar métodos numéricos sin afectar al resto del sistema (principio de abierto/cerrado).
- Garantizar que toda entrada del usuario sea validada antes de ejecutar un cálculo.
- Asegurar que la aplicación sea autocontenida: no depende de software externo (GeoGebra, Desmos, Excel) para graficar ni para calcular.
- Facilitar la trazabilidad entre requisitos funcionales y componentes de diseño, de modo que el informe técnico pueda documentar cada decisión.

## 3. Alcance funcional del sistema

El sistema debe permitir, como mínimo:

1. Ingresar una expresión matemática f(x) en una sintaxis admitida (polinomios, funciones racionales, exponenciales, logarítmicas, trigonométricas y combinaciones).
2. Seleccionar uno de los seis métodos de la Unidad 2 (o el caso de raíces múltiples).
3. Capturar los parámetros propios de cada método (intervalo [a,b], valor(es) inicial(es), función g(x), tolerancia, criterio de paro, número máximo de iteraciones).
4. Validar los datos antes de calcular.
5. Ejecutar el método, generando una tabla de iteraciones y un resultado final (raíz, iteraciones realizadas, error final, criterio de paro alcanzado).
6. Graficar f(x) dentro de la propia aplicación, señalando la raíz encontrada.
7. Permitir repetir el proceso con otra función/método sin reiniciar la aplicación.
8. Informar errores de forma comprensible (dominio, división entre cero, ausencia de cambio de signo, datos incompletos, etc.).

## 4. Lenguaje de programación y justificación

**Lenguaje:** C++ (asignado por el ciclo de lista: 1=Python, 2=HTML/JavaScript, 3=C++; posición 6 → C++).

**Justificación de diseño:**

- C++ permite construir una aplicación de escritorio autocontenida (un solo ejecutable), cumpliendo el requisito de no depender de programas externos.
- El paradigma orientado a objetos de C++ facilita modelar cada método numérico como una clase independiente que implementa una interfaz común, favoreciendo la extensibilidad.
- Existen bibliotecas gráficas que se **vinculan en tiempo de compilación** (no son programas externos que el usuario deba abrir) para construir la interfaz gráfica y el módulo de graficación integrados, como Qt (con su módulo de widgets y `QtCharts`/`QCustomPlot`) o, alternativamente, SFML para un lienzo de dibujo 2D a bajo nivel. Estas bibliotecas forman parte del propio binario de la aplicación y no constituyen una dependencia externa en el sentido que prohíbe el enunciado (GeoGebra, Desmos, Excel).
- El manejo de excepciones de C++ (`try/catch`) es adecuado para implementar la estrategia de validación y manejo de errores descrita más adelante.

## 5. Arquitectura general del sistema

Se propone una **arquitectura en capas**, inspirada en el patrón Modelo–Vista–Controlador (MVC), que separa la interfaz, el control del flujo de la aplicación y la lógica numérica/matemática.

```
┌─────────────────────────────────────────────────────────┐
│                     CAPA DE PRESENTACIÓN                  │
│   (Interfaz gráfica: menús, formularios, tabla de         │
│    iteraciones, lienzo de graficación, mensajes de error) │
└───────────────────────────▲─────────────────────────────┘
                             │ eventos / datos de entrada
┌───────────────────────────▼─────────────────────────────┐
│                   CAPA DE CONTROL (Controlador)            │
│  Orquesta el caso de uso: recibe la función y parámetros,  │
│  invoca al Validador, selecciona el Método Numérico,       │
│  recibe resultados y los envía a la Vista y al Graficador  │
└───────────┬───────────────────────────────┬──────────────┘
            │                               │
┌───────────▼─────────────┐   ┌─────────────▼───────────────┐
│   CAPA DE VALIDACIÓN      │   │     CAPA DE LÓGICA NUMÉRICA  │
│ - Parser/evaluador de f(x)│   │ - Interfaz MetodoNumerico     │
│ - Reglas de validación    │   │ - Bisección, Falsa Posición,  │
│   por método              │   │   Punto Fijo, Newton-Raphson, │
│ - Detección de dominio,   │   │   Secante, Newton Modificado  │
│   signo, división entre 0 │   │ - Generador de tabla de       │
│                            │   │   iteraciones                 │
└────────────────────────────┘   └───────────────────────────────┘
            │                               │
            └───────────────┬───────────────┘
                             ▼
              ┌───────────────────────────────┐
              │   CAPA DE GRAFICACIÓN          │
              │ - Muestreo de f(x) en un rango │
              │ - Trazo de ejes y curva        │
              │ - Marcado de la raíz aproximada│
              └───────────────────────────────┘
```

### Descripción de las capas

1. **Presentación:** ventanas/formularios donde el usuario selecciona la función, el método y los parámetros; muestra la tabla de iteraciones, el resultado final y el lienzo de la gráfica; despliega los mensajes de error.
2. **Control:** coordina el ciclo completo de un cálculo (entrada → validación → ejecución → presentación de resultados → graficación) y gestiona la posibilidad de repetir el proceso sin cerrar la aplicación.
3. **Validación:** verifica la sintaxis y el dominio de f(x), la coherencia de los parámetros (intervalo con cambio de signo, tolerancia positiva, máximo de iteraciones válido, valores iniciales dentro del dominio) antes de permitir el cálculo.
4. **Lógica numérica:** contiene la implementación conceptual de cada método como una unidad independiente, todas con la misma forma de entrada/salida (polimorfismo mediante una interfaz común).
5. **Graficación:** genera la representación visual de f(x) y la raíz dentro de la misma ventana de la aplicación, sin depender de software externo.

## 6. Diagrama de componentes

```
            ┌───────────────┐
            │  Aplicación    │  (punto de entrada)
            └──────┬────────┘
                    │
        ┌───────────┴────────────┐
        │                        │
┌───────▼────────┐      ┌────────▼─────────┐
│ InterfazUsuario │◄────►│ ControladorApp   │
└───────┬─────────┘      └────────┬─────────┘
        │                         │
        │                ┌────────┼─────────────┬─────────────────┐
        │                │        │              │                 │
        │        ┌───────▼──┐ ┌───▼──────┐ ┌─────▼───────┐ ┌───────▼───────┐
        │        │Validador │ │ Fabrica  │ │ MotorGrafico│ │ GestorErrores │
        │        └──────────┘ │ DeMetodos│ └─────────────┘ └───────────────┘
        │                     └────┬─────┘
        │                          │
        │               ┌──────────┼───────────────────────────────┐
        │               │          │           │          │         │
        │         ┌─────▼───┐ ┌────▼───┐ ┌─────▼────┐┌────▼────┐┌───▼─────────┐
        │         │Biseccion│ │FalsaPos│ │PuntoFijo ││Newton   ││Secante /    │
        │         └─────────┘ └────────┘ └──────────┘│Raphson  ││NewtonMod.   │
        │                                             └─────────┘└─────────────┘
```

Todos los métodos comparten la misma interfaz (`MetodoNumerico`), lo que permite que el `ControladorApp` los invoque de manera uniforme sin conocer los detalles internos de cada uno (polimorfismo).

## 7. Modelo de datos (entidades del dominio)

| Entidad | Descripción | Atributos conceptuales |
|---|---|---|
| **Funcion** | Representa f(x) ingresada por el usuario, ya interpretada por el parser. | Expresión original (texto), árbol de evaluación interno, dominio restringido (si aplica) |
| **ParametrosMetodo** | Agrupa los datos de entrada que requiere un método específico. | a, b (intervalo), x0, x1 (valores iniciales), g(x) (para punto fijo), tolerancia, criterio de paro, iteraciones máximas, multiplicidad esperada (raíces múltiples) |
| **IteracionResultado** | Representa una fila de la tabla de iteraciones. | Número de iteración, valores intermedios (xi, xi+1, a, b, f(xi), etc.), error aproximado |
| **ResultadoFinal** | Resumen del cálculo. | Raíz aproximada, número total de iteraciones, error final, criterio de paro alcanzado, estado (éxito/fallo) |
| **ErrorValidacion** | Representa una condición que impide el cálculo. | Tipo de error, mensaje descriptivo, campo/origen del error |
| **DatosGrafica** | Conjunto de puntos muestreados para dibujar f(x). | Rango de muestreo, pares (x, f(x)), coordenadas de la raíz encontrada |

## 8. Diseño de clases

### 8.1 Diagrama de clases (descriptivo)

```
                 ┌────────────────────┐
                 │   MetodoNumerico    │  (clase abstracta / interfaz)
                 ├────────────────────┤
                 │ + ejecutar()        │
                 │ + obtenerTabla()    │
                 │ + obtenerResultado()│
                 └─────────▲──────────┘
                            │ (herencia)
   ┌───────────┬────────────┼─────────────┬─────────────┬───────────────┐
┌──┴───┐   ┌────┴────┐  ┌────┴─────┐  ┌────┴─────┐  ┌────┴────┐  ┌───────┴────────┐
│Grafico│  │Biseccion│  │FalsaPos. │  │PuntoFijo │  │Newton   │  │Secante /       │
│(trazo)│  └─────────┘  └──────────┘  └──────────┘  │Raphson  │  │NewtonModificado│
└───────┘                                            └─────────┘  └────────────────┘

┌─────────────┐      ┌───────────────┐      ┌──────────────┐
│ Funcion      │◄────│ Validador      │────►│ GestorErrores │
└─────────────┘      └───────────────┘      └──────────────┘

┌───────────────┐      ┌─────────────┐      ┌───────────────┐
│ ControladorApp│─────►│ MotorGrafico│      │InterfazUsuario │
└───────────────┘      └─────────────┘      └───────────────┘
```

### 8.2 Responsabilidades de cada clase

**Funcion**
- Encapsula la expresión matemática ingresada por el usuario y expone una operación de evaluación `f(x)` y, cuando el método lo requiera, `f'(x)` (derivada, obtenida de forma simbólica o numérica) y `g(x)` (para punto fijo).
- Detecta en tiempo de evaluación condiciones fuera de dominio (logaritmo de valor no positivo, raíz par de negativo, división entre cero) y las reporta como una condición controlada, no como una falla del programa.

**Validador**
- Verifica la sintaxis de la expresión antes de aceptarla.
- Aplica reglas específicas según el método seleccionado: existencia de cambio de signo en [a,b] para bisección y falsa posición; convergencia esperada de g(x) para punto fijo; que la derivada no se anule en el punto inicial para Newton-Raphson; que x0 ≠ x1 para la secante; tolerancia positiva; número máximo de iteraciones entero positivo.
- Devuelve una colección de `ErrorValidacion` si algo falla; si la colección está vacía, autoriza el cálculo.

**GestorErrores**
- Centraliza la traducción de condiciones técnicas (excepciones, códigos de error) a mensajes comprensibles para el usuario.
- Clasifica los errores (sintaxis, dominio, parámetros, numéricos) para que la interfaz pueda resaltarlos de forma diferenciada.

**MetodoNumerico (interfaz común)**
- Define el contrato que deben cumplir todos los métodos: recibir una `Funcion` y sus `ParametrosMetodo`, ejecutar el proceso iterativo, y producir una lista de `IteracionResultado` junto con un `ResultadoFinal`.
- Esta interfaz es la base del polimorfismo que permite al `ControladorApp` tratar a todos los métodos de manera uniforme.

**Biseccion / FalsaPosicion**
- Reciben el intervalo [a,b]; en cada iteración calculan el punto medio (o el punto de la secante), evalúan el signo de f(x) para decidir qué subintervalo conservar, y registran el error como el tamaño del intervalo o la diferencia entre iteraciones sucesivas.

**PuntoFijo**
- Recibe x0 y la función de iteración g(x); en cada paso calcula x_{i+1} = g(x_i) y registra el error como |x_{i+1} − x_i|; detecta divergencia si el error crece de forma sostenida.

**NewtonRaphson**
- Recibe x0; en cada iteración calcula x_{i+1} = x_i − f(x_i)/f'(x_i); valida que f'(x_i) no sea numéricamente cero antes de dividir.

**Secante**
- Recibe x0 y x1; aproxima la derivada con el cociente incremental entre las dos últimas iteraciones, evitando requerir la expresión analítica de f'(x).

**NewtonRaphsonModificado**
- Variante para raíces de multiplicidad m conocida o estimada; ajusta la fórmula de iteración multiplicando el término de corrección por la multiplicidad, para recuperar la convergencia cuadrática en raíces múltiples.

**Grafico (método gráfico)**
- No calcula una raíz por iteración; en su lugar, muestrea f(x) en un rango dado y entrega los intervalos donde se detecta cambio de signo, como apoyo para elegir valores iniciales de los demás métodos.

**MotorGrafico**
- Recibe una `Funcion` y un rango de graficación, genera `DatosGrafica` (muestreo de puntos), dibuja los ejes, la curva y marca la raíz aproximada obtenida por el método ejecutado.
- Es independiente de los métodos numéricos: puede invocarse de forma aislada (método gráfico) o como complemento visual de cualquier otro método.

**ControladorApp**
- Orquesta el flujo completo: recibe la función y el método elegido desde la interfaz, delega la validación, construye el método numérico correspondiente (mediante una fábrica), ejecuta el cálculo, envía la tabla de iteraciones y el resultado a la interfaz, y solicita al `MotorGrafico` la representación visual.
- Gestiona el ciclo "repetir cálculo" devolviendo el sistema a un estado inicial sin reiniciar la aplicación.

**InterfazUsuario**
- Presenta los formularios de entrada, la tabla de iteraciones, el resultado final, el lienzo de la gráfica y los mensajes de error, y transmite las acciones del usuario al `ControladorApp`.

**FabricaDeMetodos** (patrón *Factory*)
- Dado el identificador del método elegido por el usuario, instancia la clase concreta correspondiente (`Biseccion`, `FalsaPosicion`, etc.) y la entrega al controlador a través de la interfaz `MetodoNumerico`, desacoplando al controlador de las clases concretas.

## 9. Diseño de cada método numérico (especificación funcional)

| Método | Entradas requeridas | Salida por iteración | Criterio de paro típico | Condición de error a validar |
|---|---|---|---|---|
| Gráfico | Función, rango [x_min, x_max], número de muestras | Lista de intervalos con cambio de signo | No aplica (exploratorio) | Rango inválido (x_min ≥ x_max) |
| Bisección | f(x), a, b, tolerancia, iteraciones máx. | Punto medio, f(punto medio), nuevo intervalo, error | Error < tolerancia o se alcanza el máximo de iteraciones | f(a)·f(b) > 0 (sin cambio de signo) |
| Falsa posición | f(x), a, b, tolerancia, iteraciones máx. | Punto de la secante, f(punto), nuevo intervalo, error | Error < tolerancia o máximo de iteraciones | f(a)·f(b) > 0 |
| Punto fijo | g(x), x0, tolerancia, iteraciones máx. | x_{i+1}, error | Error < tolerancia o máximo de iteraciones | Divergencia detectada (|g'(x)| > 1 estimado) |
| Newton-Raphson | f(x), f'(x), x0, tolerancia, iteraciones máx. | x_{i+1}, f(x_i), f'(x_i), error | Error < tolerancia o máximo de iteraciones | f'(x_i) ≈ 0 |
| Secante | f(x), x0, x1, tolerancia, iteraciones máx. | x_{i+1}, error | Error < tolerancia o máximo de iteraciones | f(x_i) − f(x_{i-1}) ≈ 0 |
| Newton-Raphson modificado | f(x), f'(x), f''(x) o multiplicidad m, x0, tolerancia | x_{i+1}, error | Error < tolerancia o máximo de iteraciones | Multiplicidad no válida o denominador ≈ 0 |

## 10. Flujo de operación del sistema (caso de uso principal)

Secuencia descrita para el caso de uso **"Calcular raíz de una función"**:

1. El usuario, desde la `InterfazUsuario`, ingresa la expresión de f(x) y selecciona un método.
2. La interfaz solicita los parámetros específicos del método elegido (mostrando solo los campos pertinentes).
3. El usuario captura los parámetros y confirma la ejecución.
4. El `ControladorApp` envía la función y los parámetros al `Validador`.
5. Si el `Validador` encuentra errores, el `GestorErrores` traduce cada uno a un mensaje claro y la `InterfazUsuario` los muestra; el flujo regresa al paso 2.
6. Si la validación es exitosa, el `ControladorApp` solicita a la `FabricaDeMetodos` la instancia del método correspondiente.
7. El método ejecuta su proceso iterativo, generando una `IteracionResultado` por cada paso hasta cumplir el criterio de paro o alcanzar el máximo de iteraciones.
8. El `ControladorApp` recibe la lista de iteraciones y el `ResultadoFinal`, y los envía a la `InterfazUsuario` para mostrarse como tabla y resumen.
9. El `ControladorApp` solicita al `MotorGrafico` la representación de f(x) junto con la raíz encontrada; la gráfica se dibuja dentro de la misma ventana.
10. El usuario puede repetir el proceso desde el paso 1 (nueva función/método) sin cerrar la aplicación, o finalizar.

## 11. Casos de uso del sistema

- **Ingresar función:** el usuario escribe f(x) en la sintaxis admitida.
- **Seleccionar método numérico:** el usuario elige entre los seis métodos o el caso de raíces múltiples.
- **Capturar parámetros del método:** formulario dinámico según el método seleccionado.
- **Validar datos:** ejecución automática antes de habilitar el botón de cálculo.
- **Ejecutar cálculo:** disparo del método numérico seleccionado.
- **Visualizar tabla de iteraciones:** presentación tabular del proceso.
- **Visualizar gráfica integrada:** representación de f(x) y la raíz.
- **Repetir cálculo:** reinicia el formulario sin cerrar la aplicación.
- **Gestionar errores:** presentación de mensajes comprensibles ante cualquier condición inválida.

## 12. Diseño de validación y manejo de errores

| Categoría de error | Ejemplo | Momento de detección | Respuesta del sistema |
|---|---|---|---|
| Sintaxis de la función | Paréntesis desbalanceados, operador no reconocido | Al interpretar la expresión (antes de calcular) | Mensaje señalando la posición/carácter problemático |
| Dominio de la función | ln(x) con x ≤ 0, raíz par de negativo | Al evaluar f(x) en un punto | Mensaje indicando el valor fuera de dominio y el motivo |
| Intervalo sin cambio de signo | f(a)·f(b) > 0 en bisección/falsa posición | Antes de iniciar las iteraciones | Mensaje solicitando un nuevo intervalo |
| Parámetros numéricos inválidos | Tolerancia negativa, iteraciones máx. ≤ 0, x0 = x1 en secante | Validación previa al cálculo | Mensaje específico por campo |
| División entre cero / derivada nula | f'(x_i) ≈ 0 en Newton-Raphson | Durante la iteración | Interrupción controlada del método con mensaje explicativo |
| No convergencia | Se alcanza el máximo de iteraciones sin cumplir tolerancia | Al finalizar el ciclo iterativo | Se informa el mejor resultado obtenido y que no se alcanzó la tolerancia solicitada |
| Datos incompletos | Campo requerido vacío | Validación previa al cálculo | Mensaje indicando el campo faltante |

El manejo interno se apoya en el mecanismo de excepciones de C++: las condiciones numéricas problemáticas (división entre cero, dominio inválido) se capturan dentro del propio método o de la clase `Funcion` y se transforman en objetos `ErrorValidacion` que la interfaz puede mostrar sin interrumpir la ejecución general de la aplicación.

## 13. Diseño de la interfaz de usuario

Estructura de pantallas propuesta:

1. **Pantalla principal:** campo de ingreso de f(x), lista desplegable de métodos, botón "Continuar".
2. **Panel de parámetros (dinámico):** se adapta según el método elegido, mostrando únicamente los campos necesarios (intervalo, valores iniciales, g(x), tolerancia, iteraciones máximas, multiplicidad para raíces múltiples).
3. **Panel de resultados:** tabla de iteraciones (scrollable) y resumen del resultado final (raíz, iteraciones, error, criterio de paro).
4. **Panel de gráfica:** lienzo embebido en la misma ventana, con ejes, curva de f(x) y marcador de la raíz.
5. **Barra/zona de mensajes:** área fija para mostrar errores de validación sin cerrar la ventana ni perder los datos capturados.
6. **Botón "Nuevo cálculo":** limpia el panel de parámetros y resultados, conservando la aplicación abierta.

Principios de diseño de interfaz aplicados: consistencia visual entre métodos, retroalimentación inmediata ante errores, y separación clara entre entrada, resultados y gráfica para que el usuario pueda relacionar visualmente el proceso iterativo con la curva de la función.

## 14. Diseño del módulo de graficación integrada

- El `MotorGrafico` muestrea f(x) en un rango determinado automáticamente a partir de los datos del método (p. ej., alrededor del intervalo [a,b] o de la raíz obtenida, con un margen adicional) o definido manualmente por el usuario.
- Dibuja los ejes cartesianos, la curva de f(x) y un marcador en el punto (raíz, 0).
- Se integra como un widget/lienzo dentro de la misma ventana de la aplicación (no se abre una ventana externa ni se invoca un programa de terceros), usando una biblioteca gráfica vinculada al binario de la aplicación (p. ej., Qt Widgets/QCustomPlot o SFML).
- Permite reutilizarse de forma independiente para el método gráfico (exploración de raíces) y como complemento visual del resultado de cualquiera de los otros cinco métodos.

## 15. Raíces múltiples (Newton-Raphson modificado)

- Se añade un modo de operación dentro del método Newton-Raphson en el que el usuario puede indicar que se sospecha una raíz múltiple (convergencia lenta detectada o declarada por el usuario).
- La clase `NewtonRaphsonModificado` ajusta la fórmula de iteración para restaurar la convergencia cuadrática, usando la multiplicidad m (si se conoce) o una estimación basada en f, f' y f''.
- El `Validador` verifica que, en este modo, estén disponibles las derivadas necesarias y que la multiplicidad declarada sea un entero positivo.

## 16. Plan de pruebas asociado al diseño

El diseño contempla que cada método se ejecute contra las familias de funciones exigidas por el proyecto, verificando tanto el camino exitoso como las condiciones de error:

| Tipo de función | Ejemplo sugerido | Métodos aplicables a probar |
|---|---|---|
| Polinomial/algebraica | f(x) = x³ − 4x − 9 | Bisección, Falsa posición, Newton-Raphson, Secante |
| Exponencial | f(x) = 2eˣ − 5 | Newton-Raphson, Punto fijo, Secante |
| Logarítmica (con dominio) | f(x) = ln(x) − x + 2 | Bisección, Falsa posición (validando x > 0) |
| Trigonométrica | f(x) = sen(x) − 0.5x | Punto fijo, Secante |
| Combinada/trascendente | f(x) = x²e^(−x) − 1 | Newton-Raphson, Newton-Raphson modificado (si hay raíz múltiple) |

Para cada caso se documentarán en el informe: los parámetros utilizados, la tabla de iteraciones obtenida, la gráfica generada y, cuando aplique, la respuesta del sistema ante datos inválidos (intervalos sin cambio de signo, valores fuera de dominio, tolerancias negativas, etc.).

## 17. Trazabilidad entre requisitos y diseño

| Requisito del enunciado | Componente de diseño responsable |
|---|---|
| Ingresar/seleccionar f(x) | `InterfazUsuario`, `Funcion` |
| Seleccionar método | `InterfazUsuario`, `FabricaDeMetodos` |
| Solicitar parámetros por método | Panel de parámetros dinámico, `ParametrosMetodo` |
| Validar datos antes de calcular | `Validador` |
| Detectar dominios inválidos, división entre cero, etc. | `Validador`, `Funcion`, `GestorErrores` |
| Mostrar raíz, iteraciones, error y criterio de paro | `ResultadoFinal`, panel de resultados |
| Mostrar tabla de iteraciones | `IteracionResultado`, panel de resultados |
| Graficar dentro de la aplicación | `MotorGrafico` |
| Repetir cálculos sin cerrar la aplicación | `ControladorApp` (ciclo de reinicio) |
| Mensajes de error comprensibles | `GestorErrores` |

## 18. Consideraciones de extensibilidad y mantenimiento

- La interfaz común `MetodoNumerico` permite agregar nuevos métodos numéricos en el futuro sin modificar al `ControladorApp` ni a la `InterfazUsuario`.
- La separación entre `Validador` y los métodos concretos permite enriquecer las reglas de validación (por ejemplo, nuevas condiciones de dominio) sin tocar la lógica de cálculo.
- El `MotorGrafico` es independiente de los métodos numéricos, por lo que puede evolucionar (por ejemplo, añadiendo zoom o múltiples curvas) sin afectar la lógica de los métodos.

---

**Fin del documento de diseño.**
