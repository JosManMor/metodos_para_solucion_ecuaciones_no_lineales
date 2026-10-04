# Compilar la aplicación en Windows (generar el .exe)

Esta guía es para compilar el proyecto de forma nativa en Windows, usando el
mismo código fuente (`CMakeLists.txt`, `include/`, `src/`) que ya está en el
repositorio. No se necesita Docker para esto.

## Opción recomendada: Qt Creator (la más simple)

1. **Instalar Qt**: descarga el instalador oficial desde
   https://www.qt.io/download-qt-installer y ejecútalo (requiere una cuenta
   gratuita de Qt).
2. En el instalador, dentro de la versión más reciente de **Qt 5.15.x**
   (o Qt 6, ver nota al final), marca el componente **MinGW 64-bit** (incluye
   compilador, Qt y CMake integrado) y **Qt Creator** en la sección de
   herramientas. Instala.
3. Abre **Qt Creator** → `File` → `Open File or Project...` → selecciona el
   `CMakeLists.txt` de este repositorio.
4. Qt Creator detectará el *kit* MinGW 64-bit automáticamente. Si pide elegir
   un kit, selecciona el que diga **Desktop Qt 5.15.x MinGW 64-bit**.
5. En la barra inferior izquierda, cambia el modo de build a **Release**.
6. Botón ▶ (Run) para compilar y ejecutar `metodos_numericos_gui`
   directamente. También puedes compilar solo con el martillo 🔨 (Build).

El `.exe` queda en la carpeta de build que Qt Creator crea (algo como
`build-metodos_numericos-Desktop_Qt_5_15_...-Release/metodos_numericos_gui.exe`).

## Opción alterna: línea de comandos (MinGW + CMake)

Si ya tienes Qt instalado (por el instalador anterior) pero prefieres la
terminal:

```powershell
# Agrega el MinGW y las herramientas de Qt al PATH de esta sesión
# (ajusta la ruta según dónde instalaste Qt, normalmente C:\Qt\...)
$env:PATH = "C:\Qt\5.15.2\mingw81_64\bin;C:\Qt\Tools\mingw810_64\bin;C:\Qt\Tools\CMake_64\bin;" + $env:PATH

cmake -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build build -j

# Ejecutar
.\build\metodos_numericos_gui.exe
.\build\metodos_numericos.exe
.\build\pruebas_metodos.exe
```

## Opción alterna: Visual Studio + vcpkg

Si prefieres Visual Studio en vez de MinGW:

```powershell
# Instala vcpkg una sola vez (ver https://vcpkg.io)
git clone https://github.com/microsoft/vcpkg
.\vcpkg\bootstrap-vcpkg.bat
.\vcpkg\vcpkg install qt5-base:x64-windows

cmake -B build -DCMAKE_TOOLCHAIN_FILE=C:\ruta\a\vcpkg\scripts\buildsystems\vcpkg.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

El `.exe` queda en `build\Release\metodos_numericos_gui.exe`.

## Para distribuir el .exe a otra PC sin Qt instalado

Un `.exe` de Qt no corre en otra máquina si le faltan las DLL de Qt. Usa
`windeployqt` (viene incluido con Qt) parado en la carpeta donde está el
`.exe`:

```powershell
C:\Qt\5.15.2\mingw81_64\bin\windeployqt.exe metodos_numericos_gui.exe
```

Esto copia automáticamente `Qt5Core.dll`, `Qt5Gui.dll`, `Qt5Widgets.dll`, los
plugins de plataforma, etc., junto al `.exe`. Después puedes copiar toda esa
carpeta a otra PC Windows y el programa corre sin instalar nada más.

## Notas

- La versión de consola (`metodos_numericos.exe`) y las pruebas
  (`pruebas_metodos.exe`) **no dependen de Qt**: compilan y corren igual con
  cualquier compilador (MinGW o MSVC) aunque no tengas Qt instalado, solo que
  en ese caso `cmake` no generará el target `metodos_numericos_gui` (lo indica
  en su salida) — basta con tener Qt para obtener también la GUI.
- Si usas Qt 6 en vez de Qt 5, cambia en `CMakeLists.txt` la línea
  `find_package(Qt5 COMPONENTS Widgets QUIET)` y
  `target_link_libraries(... Qt5::Widgets)` por `Qt6`/`Qt6::Widgets`; el
  resto del código es compatible con ambas versiones sin cambios.
