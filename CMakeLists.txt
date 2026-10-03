cmake_minimum_required(VERSION 3.16)
project(metodos_numericos LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE MN_SOURCES CONFIGURE_DEPENDS src/metodos_numericos/*.cpp)

add_executable(metodos_numericos src/main.cpp ${MN_SOURCES})
target_include_directories(metodos_numericos PRIVATE include)

enable_testing()
add_executable(pruebas_metodos tests/pruebas_metodos.cpp ${MN_SOURCES})
target_include_directories(pruebas_metodos PRIVATE include)
add_test(NAME pruebas_metodos COMMAND pruebas_metodos)
