# Imagen única: instala Qt5/cmake dentro del contenedor (no requiere sudo en
# el host) y compila tanto la GUI (Qt Widgets) como la versión de consola y
# las pruebas automáticas. Pensada para que el proyecto corra igual en
# cualquier máquina con Docker, sin instalar nada manualmente.
FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
        build-essential cmake qtbase5-dev qtbase5-dev-tools qt5-qmake pkg-config \
        libxcb-xinerama0 libxkbcommon-x11-0 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

RUN cmake -B build -DCMAKE_BUILD_TYPE=Release \
    && cmake --build build -j"$(nproc)"

ENV QT_QPA_PLATFORM=xcb

# Por defecto arranca la GUI (requiere compartir un servidor X del host;
# ver README.md para las instrucciones de "docker run"). Para usar solo la
# consola, sobreescribe el comando: docker run ... ./build/metodos_numericos
CMD ["./build/metodos_numericos_gui"]
