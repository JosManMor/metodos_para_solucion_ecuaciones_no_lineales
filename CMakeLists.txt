cmake_minimum_required(VERSION 3.16)
project(metodos_numericos LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Lógica de negocio pura (sin Qt): núcleo, validación y los métodos numéricos.
# La comparten la consola, la GUI y las pruebas.
file(GLOB_RECURSE CORE_SOURCES CONFIGURE_DEPENDS
    src/metodos_numericos/core/*.cpp
    src/metodos_numericos/validacion/*.cpp
    src/metodos_numericos/metodos/*.cpp
)

# ---- Aplicación de consola (siempre se puede compilar, no requiere Qt) ----
add_executable(metodos_numericos
    src/main.cpp
    src/metodos_numericos/control/ControladorApp.cpp
    src/metodos_numericos/grafica/GraficadorAscii.cpp
    src/metodos_numericos/ui/InterfazConsola.cpp
    ${CORE_SOURCES}
)
target_include_directories(metodos_numericos PRIVATE include)

# ---- Pruebas automáticas de los métodos (sin Qt) ----
enable_testing()
add_executable(pruebas_metodos tests/pruebas_metodos.cpp ${CORE_SOURCES})
target_include_directories(pruebas_metodos PRIVATE include)
add_test(NAME pruebas_metodos COMMAND pruebas_metodos)

# ---- Aplicación gráfica (Qt Widgets) ----
# Se construye si hay Qt5 o Qt6 Widgets disponible (detecta cualquiera de los
# dos automáticamente, ya que el instalador oficial de Qt hoy ofrece Qt6 por
# defecto, mientras que Docker/apt suelen traer Qt5).
find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets QUIET)
if(QT_FOUND)
    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
    message(STATUS "Qt${QT_VERSION_MAJOR} Widgets encontrado: se construirá metodos_numericos_gui")
    set(CMAKE_AUTOMOC ON)
    # Los headers con Q_OBJECT se listan explícitamente como fuentes: viven en
    # include/ (fuera del directorio de sus .cpp), y AUTOMOC solo los detecta
    # de forma automática cuando comparten carpeta con su .cpp; al estar en un
    # árbol de directorios separado hay que declararlos aquí para que se
    # ejecute moc sobre ellos.
    add_executable(metodos_numericos_gui
        src/main_gui.cpp
        src/metodos_numericos/control/ControladorApp.cpp
        src/metodos_numericos/grafica/GraficadorAscii.cpp
        src/metodos_numericos/ui/VentanaPrincipal.cpp
        src/metodos_numericos/ui/GraficoWidget.cpp
        include/metodos_numericos/ui/VentanaPrincipal.hpp
        include/metodos_numericos/ui/GraficoWidget.hpp
        ${CORE_SOURCES}
    )
    target_include_directories(metodos_numericos_gui PRIVATE include)
    target_link_libraries(metodos_numericos_gui PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
else()
    message(STATUS "Qt Widgets (5 o 6) no encontrado: no se construirá metodos_numericos_gui "
                    "(instala Qt o compila dentro del contenedor Docker)")
endif()
