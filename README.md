# Aplicación de Métodos Numéricos — Unidad 2

**Estudiante:** Carrillo Valencia Ruth · **Grupo:** AM3B · **Lenguaje:** C++

Aplicación autocontenida (no depende de GeoGebra, Desmos, Excel ni ningún otro
programa) que implementa los seis métodos de la Unidad 2 para ecuaciones no
lineales — método gráfico, bisección, falsa posición, iteración de punto
fijo, Newton-Raphson y secante — más Newton-Raphson modificado para raíces
múltiples. Incluye validación de entradas, tabla de iteraciones y una gráfica
de f(x) integrada en la propia aplicación.

Ver el diseño completo en `diseño_sistema.md`, la arquitectura y estructura de
carpetas en `arquitectura_software_carpetas.md`, y el plan de trabajo en
`plan_trabajo.md`.

## Dos interfaces, un mismo núcleo

El proyecto tiene **dos vistas** sobre la misma lógica de negocio
(`ControladorApp`, `Validador` y los siete métodos no saben cuál de las dos
las está usando):

- **GUI (Qt Widgets)** — `metodos_numericos_gui`: ventana con formulario,
  tabla de iteraciones y una gráfica real de f(x) dibujada con `QPainter`
  (sin QtCharts/QCustomPlot). Es la interfaz gráfica exigida por el proyecto.
  Requiere Qt5 (`qtbase5-dev`) para compilarse; la forma más simple de
  obtenerlo es con Docker (ver abajo), sin instalar nada en el sistema.
- **Consola** — `metodos_numericos`: mismo flujo por terminal, con una
  gráfica dibujada en texto (ASCII). No requiere Qt ni Docker; compila con
  cualquier `g++` reciente.

## Opción A — Docker (recomendada: corre igual en cualquier máquina)

La imagen instala Qt5/cmake dentro del contenedor (no se toca el sistema
anfitrión) y compila los tres binarios (GUI, consola y pruebas).

```sh
docker build -t metodos-numericos .
```

### Ejecutar la GUI

**Opción recomendada — VNC (funciona igual en Linux/Windows/macOS, incluida
Docker Desktop):** compartir el socket X11 del host con Docker Desktop no
siempre funciona porque Docker Desktop corre en una VM y ese socket no se
reenvía de forma fiable (el error típico es `could not connect to display`
o `Could not load the Qt platform plugin "xcb"`). En vez de pelear con eso,
la GUI puede servirse por red usando el plugin `vnc` de Qt:

```sh
docker run --rm -p 5900:5900 metodos-numericos ./build/metodos_numericos_gui -platform vnc
```

Y conéctate con cualquier cliente VNC a `localhost:5900` (sin usuario ni
contraseña):
- **Linux:** Remmina (protocolo VNC) u otro cliente VNC.
- **Windows:** TightVNC Viewer, RealVNC Viewer, o Remmina si usas WSL con GUI.
- **macOS:** la app "Conexión a pantalla compartida" (Screen Sharing) soporta
  `vnc://localhost:5900` directamente, o cualquier cliente VNC.

**Alternativa — X11 nativo (solo si tu Docker corre directo sobre Linux, sin
Docker Desktop):**
```sh
xhost +local:docker
docker run --rm -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix metodos-numericos
```
Si usas Docker Desktop, antes hay que compartir `/tmp` en *Settings → Resources
→ File Sharing*; aun así puede fallar por la limitación de sockets de la VM,
en cuyo caso usa la opción VNC de arriba.

### Ejecutar solo la consola (no requiere servidor X)

```sh
docker run --rm -it metodos-numericos ./build/metodos_numericos
```

### Ejecutar las pruebas automáticas

```sh
docker run --rm metodos-numericos ./build/pruebas_metodos
```

## Opción B — Compilar localmente

### B.1 — Solo la consola, con Makefile (no requiere Qt ni cmake)

```sh
make run      # compila y ejecuta la aplicación de consola
make test     # compila y corre las pruebas automáticas de los métodos
make clean    # borra los binarios generados
```

### B.2 — GUI + consola + pruebas, con CMake (requiere Qt5 instalado)

En Ubuntu/Debian: `sudo apt-get install qtbase5-dev qtbase5-dev-tools qt5-qmake cmake pkg-config`.
En Windows/macOS, ver la sección de instalación de Qt en `diseño_sistema.md`.

```sh
cmake -B build
cmake --build build
./build/metodos_numericos_gui   # GUI (si Qt5 Widgets fue encontrado por cmake)
./build/metodos_numericos       # consola
ctest --test-dir build          # pruebas automáticas
```

Si `cmake` no encuentra Qt5, construye únicamente `metodos_numericos` y
`pruebas_metodos` (lo indica en su salida), sin fallar el resto del build.

## Uso de la aplicación

1. Escribe la expresión de f(x) usando la sintaxis admitida (operadores
   `+ - * / ^`, funciones `sin cos tan asin acos atan sinh cosh tanh exp ln
   log sqrt abs`, constantes `pi` y `e`, variable `x`).
2. Elige uno de los siete métodos.
3. Captura los parámetros que pida el método (intervalo, valores iniciales,
   tolerancia, iteraciones máximas, etc.).
4. Revisa la tabla de iteraciones y el resultado final.
5. Consulta la gráfica de f(x) integrada (se actualiza automáticamente tras
   calcular; en la GUI puedes ajustar el rango y presionar "Actualizar
   gráfica").
6. Repite con otra función/método sin cerrar la aplicación ("Nuevo cálculo"
   en la GUI, o respondiendo "s" en la consola).

## Pendiente (ver `plan_trabajo.md`)

- Evidencias de prueba con todas las familias de funciones exigidas, informe
  técnico y video de presentación.
