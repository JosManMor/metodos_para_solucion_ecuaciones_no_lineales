# Aplicación de Métodos Numéricos — Unidad 2

**Estudiante:** Carrillo Valencia Ruth · **Grupo:** AM3B · **Lenguaje:** C++

Aplicación de consola, autocontenida (no depende de GeoGebra, Desmos, Excel ni
ningún otro programa), que implementa los seis métodos de la Unidad 2 para
ecuaciones no lineales — método gráfico, bisección, falsa posición, iteración
de punto fijo, Newton-Raphson y secante — más Newton-Raphson modificado para
raíces múltiples. Incluye validación de entradas, tabla de iteraciones y una
gráfica de f(x) dibujada en texto dentro de la propia terminal.

Ver el diseño completo en `diseño_sistema.md`, la arquitectura y estructura de
carpetas en `arquitectura_software_carpetas.md`, y el plan de trabajo en
`plan_trabajo.md`.

## Estado actual de la implementación

Esta primera versión implementa la **interfaz de consola** (`InterfazConsola`)
en vez de la interfaz gráfica con Qt descrita en el diseño, porque este
entorno de desarrollo no tiene instaladas las bibliotecas de Qt/cmake/pkg-config
y no fue posible instalarlas (requieren `sudo`). La lógica de negocio (núcleo,
validación, los siete métodos y la fábrica) es exactamente la que usará la
futura interfaz gráfica: `ControladorApp` y `MetodoNumerico` no saben si quien
los llama es la consola o una ventana de Qt, así que migrar a GUI más adelante
no requiere tocar los métodos numéricos.

## Compilar y ejecutar

Este repositorio puede compilarse de dos formas:

### Opción A — Makefile (no requiere cmake)

```sh
make run      # compila y ejecuta la aplicación interactiva
make test     # compila y corre las pruebas automáticas de los métodos
make clean    # borra los binarios generados
```

### Opción B — CMake (si tienes cmake instalado)

```sh
cmake -B build
cmake --build build
./build/metodos_numericos
ctest --test-dir build
```

## Uso de la aplicación

1. Escribe la expresión de f(x) usando la sintaxis admitida (se muestra al
   iniciar la aplicación: operadores `+ - * / ^`, funciones `sin cos tan asin
   acos atan sinh cosh tanh exp ln log sqrt abs`, constantes `pi` y `e`,
   variable `x`).
2. Elige uno de los siete métodos del menú.
3. Captura los parámetros que pida el método (intervalo, valores iniciales,
   tolerancia, iteraciones máximas, etc.).
4. Revisa la tabla de iteraciones y el resultado final.
5. Opcionalmente, pide ver la gráfica de f(x) integrada en la misma terminal.
6. Repite con otra función/método sin cerrar la aplicación, o escribe `salir`
   en cualquier momento para terminar.

## Pendiente (ver `plan_trabajo.md`)

- Migrar `InterfazUsuario` de consola a Qt Widgets (requiere instalar
  `qtbase5-dev`, `cmake` y `pkg-config`) y sustituir `GraficadorAscii` por un
  widget gráfico real, conforme al diseño original.
- Evidencias de prueba con todas las familias de funciones exigidas, informe
  técnico y video de presentación.
