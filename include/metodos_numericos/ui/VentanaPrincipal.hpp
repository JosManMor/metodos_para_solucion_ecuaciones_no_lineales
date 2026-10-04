#pragma once

#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QTableWidget>
#include <QWidget>
#include <optional>

#include "metodos_numericos/control/ControladorApp.hpp"
#include "metodos_numericos/ui/GraficoWidget.hpp"

namespace mn::ui {

// Vista Qt: reemplaza a InterfazConsola sin tocar el controlador ni los
// métodos numéricos. Solo conoce a ControladorApp.
class VentanaPrincipal : public QMainWindow {
    Q_OBJECT
public:
    explicit VentanaPrincipal(QWidget* parent = nullptr);

private slots:
    void actualizarCamposVisibles();
    void onCalcular();
    void onNuevo();
    void onActualizarGrafica();

private:
    mn::control::ControladorApp controlador_;

    QLineEdit* campoFuncion_;
    QComboBox* comboMetodo_;

    QLineEdit* edA_;
    QLineEdit* edB_;
    QLineEdit* edX0_;
    QLineEdit* edX1_;
    QLineEdit* edG_;
    QLineEdit* edTol_;
    QLineEdit* edIterMax_;
    QCheckBox* chkMultiplicidadConocida_;
    QLineEdit* edMultiplicidad_;

    QWidget* filaA_;
    QWidget* filaB_;
    QWidget* filaX0_;
    QWidget* filaX1_;
    QWidget* filaG_;
    QWidget* filaTol_;
    QWidget* filaIterMax_;
    QWidget* filaMultiplicidad_;

    QPushButton* btnCalcular_;
    QPushButton* btnNuevo_;

    QLabel* lblError_;
    QTableWidget* tabla_;
    QLabel* lblResumen_;

    GraficoWidget* grafico_;
    QLineEdit* edXMinGrafica_;
    QLineEdit* edXMaxGrafica_;
    QPushButton* btnActualizarGrafica_;

    std::optional<std::string> ultimaExpresionF_;
    std::optional<mn::core::ResultadoFinal> ultimoResultado_;

    QWidget* filaCampo(const QString& etiqueta, QWidget* campo);
    std::optional<mn::control::SolicitudCalculo> construirSolicitud(QString& mensajeError);
    void mostrarRespuesta(const mn::control::RespuestaCalculo& respuesta);
    void limpiarResultados();
    void actualizarGraficaConValores(double xMin, double xMax);
};

} // namespace mn::ui
