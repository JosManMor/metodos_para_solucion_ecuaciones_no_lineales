#include "metodos_numericos/ui/GraficoWidget.hpp"

#include <algorithm>
#include <cmath>
#include <vector>

#include <QPainter>
#include <QPainterPath>

namespace mn::ui {

using mn::core::Funcion;

namespace {
constexpr int MARGEN = 36;
}

GraficoWidget::GraficoWidget(QWidget* parent) : QWidget(parent) {
    setMinimumHeight(260);
    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::white);
    setPalette(pal);
}

void GraficoWidget::mostrar(const Funcion& f, double xMin, double xMax, std::optional<double> raiz) {
    funcion_ = f;
    xMin_ = xMin;
    xMax_ = xMax;
    raiz_ = raiz;
    update();
}

void GraficoWidget::limpiar() {
    funcion_.reset();
    raiz_.reset();
    update();
}

void GraficoWidget::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.fillRect(rect(), Qt::white);
    p.setPen(QPen(Qt::darkGray));
    p.drawRect(rect().adjusted(0, 0, -1, -1));

    if (!funcion_.has_value() || xMax_ <= xMin_) {
        p.drawText(rect(), Qt::AlignCenter, "Aún no hay gráfica. Ejecuta un cálculo para visualizar f(x).");
        return;
    }

    const int anchoPx = width() - 2 * MARGEN;
    const int altoPx = height() - 2 * MARGEN;
    if (anchoPx <= 10 || altoPx <= 10) return;

    const int muestras = std::max(50, anchoPx);
    std::vector<double> xs(muestras), ys(muestras);
    std::vector<bool> valido(muestras, false);
    std::vector<double> validas;
    double paso = (xMax_ - xMin_) / static_cast<double>(muestras - 1);

    for (int i = 0; i < muestras; ++i) {
        double x = xMin_ + i * paso;
        xs[i] = x;
        try {
            double y = funcion_->evaluar(x);
            if (std::isfinite(y)) {
                ys[i] = y;
                valido[i] = true;
                validas.push_back(y);
            }
        } catch (...) {
            valido[i] = false;
        }
    }

    if (validas.empty()) {
        p.drawText(rect(), Qt::AlignCenter, "No fue posible evaluar f(x) en el rango indicado.");
        return;
    }

    std::vector<double> ordenadas = validas;
    std::sort(ordenadas.begin(), ordenadas.end());
    auto percentil = [&](double q) {
        double idx = q * (static_cast<double>(ordenadas.size()) - 1);
        size_t i0 = static_cast<size_t>(std::floor(idx));
        size_t i1 = static_cast<size_t>(std::ceil(idx));
        if (i1 >= ordenadas.size()) i1 = ordenadas.size() - 1;
        double frac = idx - static_cast<double>(i0);
        return ordenadas[i0] + frac * (ordenadas[i1] - ordenadas[i0]);
    };
    double yMin = percentil(0.02);
    double yMax = percentil(0.98);
    if (yMax - yMin < 1e-9) {
        yMin -= 1.0;
        yMax += 1.0;
    } else {
        double margen = (yMax - yMin) * 0.1;
        yMin -= margen;
        yMax += margen;
    }

    auto pxX = [&](double x) { return MARGEN + (x - xMin_) / (xMax_ - xMin_) * anchoPx; };
    auto pxY = [&](double y) {
        double yc = std::clamp(y, yMin, yMax);
        return MARGEN + altoPx - (yc - yMin) / (yMax - yMin) * altoPx;
    };

    // Ejes de referencia.
    p.setPen(QPen(Qt::lightGray, 1));
    if (yMin <= 0.0 && yMax >= 0.0) {
        double y0 = pxY(0.0);
        p.drawLine(QPointF(MARGEN, y0), QPointF(MARGEN + anchoPx, y0));
    }
    if (xMin_ <= 0.0 && xMax_ >= 0.0) {
        double x0 = pxX(0.0);
        p.drawLine(QPointF(x0, MARGEN), QPointF(x0, MARGEN + altoPx));
    }

    // Curva de f(x): se corta el trazo donde f(x) no está definida.
    p.setPen(QPen(QColor("#2563eb"), 2));
    QPainterPath trazo;
    bool enTrazo = false;
    for (int i = 0; i < muestras; ++i) {
        if (!valido[i]) {
            enTrazo = false;
            continue;
        }
        QPointF pt(pxX(xs[i]), pxY(ys[i]));
        if (!enTrazo) {
            trazo.moveTo(pt);
            enTrazo = true;
        } else {
            trazo.lineTo(pt);
        }
    }
    p.drawPath(trazo);

    // Marcador de la raíz, si cae dentro del rango visible.
    if (raiz_.has_value() && *raiz_ >= xMin_ && *raiz_ <= xMax_) {
        QPointF centro(pxX(*raiz_), pxY(0.0));
        p.setPen(QPen(QColor("#dc2626"), 2));
        p.setBrush(QColor("#dc2626"));
        p.drawEllipse(centro, 4, 4);
        p.drawLine(centro - QPointF(0, 8), centro + QPointF(0, 8));
        p.drawLine(centro - QPointF(8, 0), centro + QPointF(8, 0));
    }

    // Etiquetas de los extremos de los ejes.
    p.setPen(QPen(Qt::darkGray));
    p.drawText(QRectF(0, height() - 18, MARGEN + 40, 18), Qt::AlignLeft, QString::number(xMin_, 'g', 4));
    p.drawText(QRectF(width() - MARGEN - 40, height() - 18, MARGEN + 40, 18), Qt::AlignRight,
               QString::number(xMax_, 'g', 4));
    p.drawText(QRectF(0, 0, MARGEN + 40, 16), Qt::AlignLeft, QString::number(yMax, 'g', 4));
    p.drawText(QRectF(0, height() - 2 * MARGEN, MARGEN + 40, 16), Qt::AlignLeft, QString::number(yMin, 'g', 4));
}

} // namespace mn::ui
