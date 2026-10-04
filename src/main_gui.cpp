#include <cstdlib>

#include <QApplication>
#include <QTimer>

#include "metodos_numericos/ui/VentanaPrincipal.hpp"

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    mn::ui::VentanaPrincipal ventana;
    ventana.show();

    // Utilidad de depuración/documentación: si se define MN_SCREENSHOT con
    // una ruta, la aplicación guarda una captura de la ventana y termina.
    // Pensada para generar capturas para el informe técnico sin interacción.
    if (const char* ruta = std::getenv("MN_SCREENSHOT")) {
        QString rutaCaptura(ruta);
        QTimer::singleShot(300, &ventana, [&ventana, rutaCaptura]() {
            ventana.grab().save(rutaCaptura);
            qApp->quit();
        });
    }

    return app.exec();
}
