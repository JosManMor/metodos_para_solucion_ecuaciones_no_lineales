#include "metodos_numericos/grafica/GraficadorAscii.hpp"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <vector>

namespace mn::grafica {

using mn::core::Funcion;

GraficadorAscii::GraficadorAscii(int ancho, int alto) : ancho_(ancho), alto_(alto) {}

std::string GraficadorAscii::graficar(const Funcion& f, double xMin, double xMax,
                                       std::optional<double> raiz) const {
    std::vector<double> xs(ancho_);
    std::vector<double> ys(ancho_, std::nan(""));
    double paso = (xMax - xMin) / static_cast<double>(ancho_ - 1);

    std::vector<double> validas;
    for (int c = 0; c < ancho_; ++c) {
        double x = xMin + c * paso;
        xs[c] = x;
        try {
            double y = f.evaluar(x);
            if (std::isfinite(y)) {
                ys[c] = y;
                validas.push_back(y);
            }
        } catch (...) {
            // Punto fuera de dominio: se deja como "no definido" (nan) en la gráfica.
        }
    }

    if (validas.empty()) {
        return "No fue posible evaluar f(x) en ningún punto del rango [" +
               std::to_string(xMin) + ", " + std::to_string(xMax) +
               "]. Verifica el dominio de la función y el rango elegido.";
    }

    // Recorte robusto por percentiles para que una asíntota no aplaste la escala.
    std::vector<double> ordenadas = validas;
    std::sort(ordenadas.begin(), ordenadas.end());
    auto percentil = [&](double p) {
        double idx = p * (static_cast<double>(ordenadas.size()) - 1);
        size_t i0 = static_cast<size_t>(std::floor(idx));
        size_t i1 = static_cast<size_t>(std::ceil(idx));
        if (i1 >= ordenadas.size()) i1 = ordenadas.size() - 1;
        double frac = idx - static_cast<double>(i0);
        return ordenadas[i0] + frac * (ordenadas[i1] - ordenadas[i0]);
    };
    double yMin = percentil(0.03);
    double yMax = percentil(0.97);
    if (yMax - yMin < 1e-9) {
        yMin -= 1.0;
        yMax += 1.0;
    } else {
        double margen = (yMax - yMin) * 0.08;
        yMin -= margen;
        yMax += margen;
    }

    std::vector<std::string> grid(alto_, std::string(ancho_, ' '));

    auto filaDeY = [&](double y) {
        double t = (y - yMin) / (yMax - yMin);
        int fila = static_cast<int>(std::round((1.0 - t) * (alto_ - 1)));
        return std::clamp(fila, 0, alto_ - 1);
    };
    auto colDeX = [&](double x) {
        double t = (x - xMin) / (xMax - xMin);
        int col = static_cast<int>(std::round(t * (ancho_ - 1)));
        return std::clamp(col, 0, ancho_ - 1);
    };

    // Ejes de referencia.
    if (yMin <= 0.0 && yMax >= 0.0) {
        int filaEjeX = filaDeY(0.0);
        for (int c = 0; c < ancho_; ++c) grid[filaEjeX][c] = '-';
    }
    if (xMin <= 0.0 && xMax >= 0.0) {
        int colEjeY = colDeX(0.0);
        for (int r = 0; r < alto_; ++r) {
            if (grid[r][colEjeY] == '-') grid[r][colEjeY] = '+';
            else grid[r][colEjeY] = '|';
        }
    }

    // Curva de f(x).
    for (int c = 0; c < ancho_; ++c) {
        if (std::isnan(ys[c])) continue;
        double yClip = std::clamp(ys[c], yMin, yMax);
        int fila = filaDeY(yClip);
        grid[fila][c] = '*';
    }

    // Marcador de la raíz, si se proporciona y cae dentro del rango.
    if (raiz.has_value() && *raiz >= xMin && *raiz <= xMax) {
        int col = colDeX(*raiz);
        int fila = filaDeY(0.0);
        grid[fila][col] = 'R';
    }

    std::ostringstream out;
    out << std::fixed << std::setprecision(4);
    out << "y_max ~ " << yMax << "\n";
    for (const auto& fila : grid) out << fila << "\n";
    out << "y_min ~ " << yMin << "\n";
    out << "x_min = " << xMin << std::string(std::max(0, ancho_ - 24), ' ')
        << "x_max = " << xMax << "\n";
    out << "Leyenda: '*' = f(x)   '-'/'|'/'+' = ejes   'R' = raiz aproximada\n";
    return out.str();
}

} // namespace mn::grafica
