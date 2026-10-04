#include "metodos_numericos/ui/VentanaPrincipal.hpp"

#include <algorithm>
#include <array>

#include <QHBoxLayout>
#include <QHeaderView>
#include <QVBoxLayout>

#include "metodos_numericos/core/Funcion.hpp"

namespace mn::ui {

using mn::control::RespuestaCalculo;
using mn::control::SolicitudCalculo;
using mn::core::ParametrosMetodo;
using mn::metodos::TipoMetodo;

namespace {

constexpr std::array<TipoMetodo, 7> kMapaMetodos = {
    TipoMetodo::GRAFICO,        TipoMetodo::BISECCION, TipoMetodo::FALSA_POSICION,
    TipoMetodo::PUNTO_FIJO,     TipoMetodo::NEWTON_RAPHSON, TipoMetodo::SECANTE,
    TipoMetodo::NEWTON_RAPHSON_MODIFICADO,
};

TipoMetodo tipoDesdeIndice(int indice) {
    return kMapaMetodos[static_cast<size_t>(std::clamp(indice, 0, 6))];
}

} // namespace

VentanaPrincipal::VentanaPrincipal(QWidget* parent) : QMainWindow(parent) {
    auto* central = new QWidget(this);
    auto* layoutPrincipal = new QVBoxLayout(central);

    auto* titulo = new QLabel(
        "<b>Métodos Numéricos — Unidad 2 (Ecuaciones no lineales)</b><br>"
        "Carrillo Valencia Ruth · Grupo AM3B · C++");
    layoutPrincipal->addWidget(titulo);

    auto* ayuda = new QLabel(
        "Sintaxis de f(x): operadores + - * / ^ ; funciones sin cos tan asin acos atan sinh cosh "
        "tanh exp ln log sqrt abs ; constantes pi, e ; variable x.\n"
        "Ejemplos: x^3 - 4*x - 9    2*exp(x) - 5    ln(x) - x + 2    sin(x) - 0.5*x");
    ayuda->setWordWrap(true);
    ayuda->setStyleSheet("color: #555;");
    layoutPrincipal->addWidget(ayuda);

    auto* filaFuncion = new QHBoxLayout();
    filaFuncion->addWidget(new QLabel("f(x) ="));
    campoFuncion_ = new QLineEdit();
    campoFuncion_->setPlaceholderText("Ejemplo: x^3 - 4*x - 9");
    filaFuncion->addWidget(campoFuncion_);
    filaFuncion->addWidget(new QLabel("Método:"));
    comboMetodo_ = new QComboBox();
    comboMetodo_->addItem("Método gráfico");
    comboMetodo_->addItem("Bisección");
    comboMetodo_->addItem("Falsa posición");
    comboMetodo_->addItem("Iteración de punto fijo");
    comboMetodo_->addItem("Newton-Raphson");
    comboMetodo_->addItem("Secante");
    comboMetodo_->addItem("Newton-Raphson modificado (raíces múltiples)");
    filaFuncion->addWidget(comboMetodo_);
    layoutPrincipal->addLayout(filaFuncion);

    edA_ = new QLineEdit();
    edB_ = new QLineEdit();
    edX0_ = new QLineEdit();
    edX1_ = new QLineEdit();
    edG_ = new QLineEdit();
    edG_->setPlaceholderText("g(x) despejada de f(x) = 0");
    edTol_ = new QLineEdit("0.0001");
    edIterMax_ = new QLineEdit("50");
    edMultiplicidad_ = new QLineEdit();
    edMultiplicidad_->setEnabled(false);
    chkMultiplicidadConocida_ = new QCheckBox("Conozco la multiplicidad m");

    filaA_ = filaCampo("a / x mínimo =", edA_);
    filaB_ = filaCampo("b / x máximo =", edB_);
    filaX0_ = filaCampo("x0 =", edX0_);
    filaX1_ = filaCampo("x1 =", edX1_);
    filaG_ = filaCampo("g(x) =", edG_);
    filaTol_ = filaCampo("Tolerancia =", edTol_);
    filaIterMax_ = filaCampo("Iteraciones máximas =", edIterMax_);

    auto* filaMultLayout = new QHBoxLayout();
    filaMultLayout->setContentsMargins(0, 0, 0, 0);
    filaMultLayout->addWidget(chkMultiplicidadConocida_);
    filaMultLayout->addWidget(new QLabel("m ="));
    filaMultLayout->addWidget(edMultiplicidad_);
    filaMultiplicidad_ = new QWidget();
    filaMultiplicidad_->setLayout(filaMultLayout);
    connect(chkMultiplicidadConocida_, &QCheckBox::toggled, edMultiplicidad_, &QWidget::setEnabled);

    layoutPrincipal->addWidget(filaA_);
    layoutPrincipal->addWidget(filaB_);
    layoutPrincipal->addWidget(filaX0_);
    layoutPrincipal->addWidget(filaX1_);
    layoutPrincipal->addWidget(filaG_);
    layoutPrincipal->addWidget(filaTol_);
    layoutPrincipal->addWidget(filaIterMax_);
    layoutPrincipal->addWidget(filaMultiplicidad_);

    auto* filaBotones = new QHBoxLayout();
    btnCalcular_ = new QPushButton("Calcular");
    btnNuevo_ = new QPushButton("Nuevo cálculo");
    filaBotones->addWidget(btnCalcular_);
    filaBotones->addWidget(btnNuevo_);
    filaBotones->addStretch();
    layoutPrincipal->addLayout(filaBotones);

    lblError_ = new QLabel();
    lblError_->setStyleSheet(
        "color: #b91c1c; background: #fef2f2; padding: 6px; border: 1px solid #fecaca; border-radius: 4px;");
    lblError_->setWordWrap(true);
    lblError_->hide();
    layoutPrincipal->addWidget(lblError_);

    tabla_ = new QTableWidget();
    tabla_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tabla_->horizontalHeader()->setStretchLastSection(true);
    layoutPrincipal->addWidget(tabla_, 1);

    lblResumen_ = new QLabel();
    lblResumen_->setWordWrap(true);
    lblResumen_->setStyleSheet("font-family: monospace;");
    layoutPrincipal->addWidget(lblResumen_);

    grafico_ = new GraficoWidget();
    layoutPrincipal->addWidget(grafico_, 1);

    auto* filaGrafica = new QHBoxLayout();
    filaGrafica->addWidget(new QLabel("Rango gráfica  x mín:"));
    edXMinGrafica_ = new QLineEdit();
    filaGrafica->addWidget(edXMinGrafica_);
    filaGrafica->addWidget(new QLabel("x máx:"));
    edXMaxGrafica_ = new QLineEdit();
    filaGrafica->addWidget(edXMaxGrafica_);
    btnActualizarGrafica_ = new QPushButton("Actualizar gráfica");
    filaGrafica->addWidget(btnActualizarGrafica_);
    layoutPrincipal->addLayout(filaGrafica);

    setCentralWidget(central);
    setWindowTitle("Métodos Numéricos — Unidad 2 (Ruth Carrillo Valencia)");
    resize(900, 780);

    connect(comboMetodo_, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &VentanaPrincipal::actualizarCamposVisibles);
    connect(btnCalcular_, &QPushButton::clicked, this, &VentanaPrincipal::onCalcular);
    connect(btnNuevo_, &QPushButton::clicked, this, &VentanaPrincipal::onNuevo);
    connect(btnActualizarGrafica_, &QPushButton::clicked, this, &VentanaPrincipal::onActualizarGrafica);

    actualizarCamposVisibles();
}

QWidget* VentanaPrincipal::filaCampo(const QString& etiqueta, QWidget* campo) {
    auto* contenedor = new QWidget();
    auto* lay = new QHBoxLayout(contenedor);
    lay->setContentsMargins(0, 0, 0, 0);
    auto* lbl = new QLabel(etiqueta);
    lbl->setMinimumWidth(150);
    lay->addWidget(lbl);
    lay->addWidget(campo);
    return contenedor;
}

void VentanaPrincipal::actualizarCamposVisibles() {
    TipoMetodo tipo = tipoDesdeIndice(comboMetodo_->currentIndex());

    bool esIntervalo = (tipo == TipoMetodo::GRAFICO || tipo == TipoMetodo::BISECCION ||
                         tipo == TipoMetodo::FALSA_POSICION);
    filaA_->setVisible(esIntervalo);
    filaB_->setVisible(esIntervalo);

    filaX0_->setVisible(tipo == TipoMetodo::PUNTO_FIJO || tipo == TipoMetodo::NEWTON_RAPHSON ||
                         tipo == TipoMetodo::SECANTE || tipo == TipoMetodo::NEWTON_RAPHSON_MODIFICADO);
    filaX1_->setVisible(tipo == TipoMetodo::SECANTE);
    filaG_->setVisible(tipo == TipoMetodo::PUNTO_FIJO);

    bool requiereToleranciaEIteraciones = (tipo != TipoMetodo::GRAFICO);
    filaTol_->setVisible(requiereToleranciaEIteraciones);
    filaIterMax_->setVisible(requiereToleranciaEIteraciones);

    filaMultiplicidad_->setVisible(tipo == TipoMetodo::NEWTON_RAPHSON_MODIFICADO);
}

std::optional<SolicitudCalculo> VentanaPrincipal::construirSolicitud(QString& mensajeError) {
    QString expresionF = campoFuncion_->text().trimmed();
    if (expresionF.isEmpty()) {
        mensajeError = "Ingresa la función f(x) antes de calcular.";
        return std::nullopt;
    }

    TipoMetodo tipo = tipoDesdeIndice(comboMetodo_->currentIndex());
    SolicitudCalculo sol;
    sol.expresionF = expresionF.toStdString();
    sol.tipo = tipo;
    ParametrosMetodo& p = sol.params;

    bool ok = true;
    auto leerD = [&](QLineEdit* ed, const QString& nombre) -> double {
        if (!ok) return 0.0;
        bool exito = false;
        double v = ed->text().trimmed().toDouble(&exito);
        if (!exito) {
            ok = false;
            mensajeError = QString("El campo \"%1\" debe ser un número válido.").arg(nombre);
        }
        return v;
    };
    auto leerI = [&](QLineEdit* ed, const QString& nombre) -> int {
        if (!ok) return 0;
        bool exito = false;
        int v = ed->text().trimmed().toInt(&exito);
        if (!exito) {
            ok = false;
            mensajeError = QString("El campo \"%1\" debe ser un número entero válido.").arg(nombre);
        }
        return v;
    };

    switch (tipo) {
        case TipoMetodo::GRAFICO:
            p.a = leerD(edA_, "x mínimo");
            p.b = leerD(edB_, "x máximo");
            break;
        case TipoMetodo::BISECCION:
        case TipoMetodo::FALSA_POSICION:
            p.a = leerD(edA_, "a");
            p.b = leerD(edB_, "b");
            p.tolerancia = leerD(edTol_, "tolerancia");
            p.iteracionesMaximas = leerI(edIterMax_, "iteraciones máximas");
            break;
        case TipoMetodo::PUNTO_FIJO: {
            QString g = edG_->text().trimmed();
            if (g.isEmpty()) {
                ok = false;
                mensajeError = "Ingresa g(x), la forma despejada de f(x) = 0.";
            }
            sol.expresionG = g.toStdString();
            p.x0 = leerD(edX0_, "x0");
            p.tolerancia = leerD(edTol_, "tolerancia");
            p.iteracionesMaximas = leerI(edIterMax_, "iteraciones máximas");
            break;
        }
        case TipoMetodo::NEWTON_RAPHSON:
            p.x0 = leerD(edX0_, "x0");
            p.tolerancia = leerD(edTol_, "tolerancia");
            p.iteracionesMaximas = leerI(edIterMax_, "iteraciones máximas");
            break;
        case TipoMetodo::SECANTE:
            p.x0 = leerD(edX0_, "x0");
            p.x1 = leerD(edX1_, "x1");
            p.tolerancia = leerD(edTol_, "tolerancia");
            p.iteracionesMaximas = leerI(edIterMax_, "iteraciones máximas");
            break;
        case TipoMetodo::NEWTON_RAPHSON_MODIFICADO:
            p.x0 = leerD(edX0_, "x0");
            if (chkMultiplicidadConocida_->isChecked()) {
                p.multiplicidad = leerI(edMultiplicidad_, "multiplicidad m");
            }
            p.tolerancia = leerD(edTol_, "tolerancia");
            p.iteracionesMaximas = leerI(edIterMax_, "iteraciones máximas");
            break;
    }

    if (!ok) return std::nullopt;
    return sol;
}

void VentanaPrincipal::mostrarRespuesta(const RespuestaCalculo& respuesta) {
    if (!respuesta.exito) {
        lblError_->setText(QString::fromStdString(respuesta.mensajeError));
        lblError_->show();
        tabla_->setRowCount(0);
        tabla_->setColumnCount(0);
        lblResumen_->clear();
        return;
    }
    lblError_->hide();

    QStringList encabezados;
    for (const auto& c : respuesta.columnas) encabezados << QString::fromStdString(c);
    tabla_->setColumnCount(encabezados.size());
    tabla_->setHorizontalHeaderLabels(encabezados);
    tabla_->setRowCount(static_cast<int>(respuesta.tabla.size()));
    for (size_t i = 0; i < respuesta.tabla.size(); ++i) {
        const auto& fila = respuesta.tabla[i];
        tabla_->setItem(static_cast<int>(i), 0, new QTableWidgetItem(QString::number(fila.numero)));
        for (size_t j = 0; j < fila.valores.size(); ++j) {
            auto* item = new QTableWidgetItem(QString::number(fila.valores[j].second, 'f', 6));
            tabla_->setItem(static_cast<int>(i), static_cast<int>(j) + 1, item);
        }
    }

    lblResumen_->setText(
        QString("Resultado final:\n"
                "  Éxito:            %1\n"
                "  Raíz aproximada:  %2\n"
                "  Iteraciones:      %3\n"
                "  Error final:      %4\n"
                "  Criterio de paro: %5\n"
                "  Mensaje:          %6")
            .arg(respuesta.resultado.exito ? "sí" : "no")
            .arg(respuesta.resultado.raiz, 0, 'f', 6)
            .arg(respuesta.resultado.iteraciones)
            .arg(respuesta.resultado.errorFinal, 0, 'f', 6)
            .arg(QString::fromStdString(respuesta.resultado.criterioParoAlcanzado))
            .arg(QString::fromStdString(respuesta.resultado.mensaje)));
}

void VentanaPrincipal::actualizarGraficaConValores(double xMin, double xMax) {
    if (!ultimaExpresionF_.has_value() || !(xMin < xMax)) return;
    try {
        mn::core::Funcion f(*ultimaExpresionF_);
        std::optional<double> raiz =
            (ultimoResultado_.has_value() && ultimoResultado_->exito) ? std::optional<double>(ultimoResultado_->raiz)
                                                                       : std::nullopt;
        grafico_->mostrar(f, xMin, xMax, raiz);
    } catch (const std::exception&) {
        // La expresión ya fue validada antes de llegar aquí; si algo falla, se deja la gráfica anterior.
    }
}

void VentanaPrincipal::onCalcular() {
    QString mensajeError;
    auto solicitudOpt = construirSolicitud(mensajeError);
    if (!solicitudOpt.has_value()) {
        lblError_->setText(mensajeError);
        lblError_->show();
        return;
    }

    ultimaExpresionF_ = solicitudOpt->expresionF;
    RespuestaCalculo respuesta = controlador_.ejecutarCalculo(*solicitudOpt);
    mostrarRespuesta(respuesta);

    if (respuesta.exito) {
        ultimoResultado_ = respuesta.resultado;

        double xMin, xMax;
        const ParametrosMetodo& p = solicitudOpt->params;
        if (p.a.has_value() && p.b.has_value()) {
            double margen = std::max(1.0, (*p.b - *p.a) * 0.3);
            xMin = *p.a - margen;
            xMax = *p.b + margen;
        } else {
            xMin = respuesta.resultado.raiz - 10.0;
            xMax = respuesta.resultado.raiz + 10.0;
        }
        edXMinGrafica_->setText(QString::number(xMin, 'g', 6));
        edXMaxGrafica_->setText(QString::number(xMax, 'g', 6));
        actualizarGraficaConValores(xMin, xMax);
    } else {
        ultimoResultado_.reset();
        grafico_->limpiar();
    }
}

void VentanaPrincipal::onActualizarGrafica() {
    bool okMin = false, okMax = false;
    double xMin = edXMinGrafica_->text().trimmed().toDouble(&okMin);
    double xMax = edXMaxGrafica_->text().trimmed().toDouble(&okMax);
    if (!okMin || !okMax || !(xMin < xMax)) {
        lblError_->setText("El rango de la gráfica es inválido: ingresa x mínimo < x máximo, ambos numéricos.");
        lblError_->show();
        return;
    }
    lblError_->hide();
    actualizarGraficaConValores(xMin, xMax);
}

void VentanaPrincipal::limpiarResultados() {
    tabla_->setRowCount(0);
    tabla_->setColumnCount(0);
    lblResumen_->clear();
    lblError_->hide();
}

void VentanaPrincipal::onNuevo() {
    campoFuncion_->clear();
    edA_->clear();
    edB_->clear();
    edX0_->clear();
    edX1_->clear();
    edG_->clear();
    edTol_->setText("0.0001");
    edIterMax_->setText("50");
    chkMultiplicidadConocida_->setChecked(false);
    edMultiplicidad_->clear();
    edXMinGrafica_->clear();
    edXMaxGrafica_->clear();
    comboMetodo_->setCurrentIndex(0);

    actualizarCamposVisibles();
    limpiarResultados();
    grafico_->limpiar();
    ultimaExpresionF_.reset();
    ultimoResultado_.reset();
}

} // namespace mn::ui
