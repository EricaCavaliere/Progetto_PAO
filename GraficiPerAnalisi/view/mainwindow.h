#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "datigrafico.h"
#include <QMainWindow>
#include <QSplitter>
#include <QTableView>
#include <QChartView>
#include <QPair>
#include <QActionGroup>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* = 0);
    explicit MainWindow(DatiGrafico*,QWidget* = 0);
    ~MainWindow();

    void aggiungiGrafico(DatiGrafico*);

private:
    void impostaMenuBar(QMenuBar*);
    QSplitter* impostaGrafico(DatiGrafico*,QWidget*);
    QWidget* baseTabella(DatiGrafico*,QWidget*);
    QTableView* creaTabella(DatiGrafico*,QWidget*);
    QtCharts::QChartView* creaGraficoLinea(DatiGrafico*);
    QtCharts::QChartView* creaGraficoPunti(DatiGrafico*);
    QtCharts::QChartView* creaGraficoTorta(DatiGrafico*);

    QActionGroup* legenda;
    QAction* animazione;
    QTabWidget *tab;
    QStatusBar *status;

    QVector<DatiGrafico*> window;
    QVector<QTableView*> tabelle;
    QVector<QtCharts::QChartView*> grafici;

private slots:
    void menu_file_nuovo();
    void menu_file_apri();
    void menu_file_salva();
    void menu_file_salvaTutto();
    void updateUI();
    void menu_altro_impostazioni();
    void menu_altro_info();
    void pulsante_aggiungiColonna();
    void pulsante_aggiungiRiga();
    void pulsante_eliminaColonna();
    void pulsante_eliminaRiga();
    void modificaCella();
};

#endif // MAINWINDOW_H
