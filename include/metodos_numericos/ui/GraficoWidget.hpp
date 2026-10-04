#pragma once

#include <QWidget>
#include <optional>

#include "metodos_numericos/core/Funcion.hpp"

namespace mn::ui {

// Widget propio que dibuja f(x) con QPainter (sin QtCharts/QCustomPlot):
// ejes, curva muestreada y un marcador de la raíz. Es la "gráfica integrada"
// exigida por el proyecto, embebida en la misma ventana de la aplicación.
class GraficoWidget : public QWidget {
    Q_OBJECT
public:
    explicit GraficoWidget(QWidget* parent = nullptr);

    void mostrar(const mn::core::Funcion& f, double xMin, double xMax,
                 std::optional<double> raiz = std::nullopt);
    void limpiar();

protected:
    void paintEvent(QPaintEvent* evento) override;

private:
    std::optional<mn::core::Funcion> funcion_;
    double xMin_ = -10.0;
    double xMax_ = 10.0;
    std::optional<double> raiz_;
};

} // namespace mn::ui
