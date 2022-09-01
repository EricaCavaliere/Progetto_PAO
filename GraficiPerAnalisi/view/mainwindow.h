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
    QtCharts::QChart* aggiornaGraficoLinea(int);
    QtCharts::QChart* aggiornaGraficoPunti(int);
    QtCharts::QChart* aggiornaGraficoTorta(int);

    QActionGroup* legenda;
    QAction* animazione;
    QTabWidget *tab;

    QVector<DatiGrafico*> window;
    QVector<QTableView*> tabelle;
    QVector<QtCharts::QChartView*> grafici;

signals:
    void nuovo();
    void apri();
    void salva(QVector<DatiGrafico*>);

private slots:
    void menu_file_nuovo();
    void menu_file_apri();
    void menu_file_salva();
    void menu_file_rinomina();
    void updateUI();
    void updateUIanimation();
    void pulsante_aggiungiColonna();
    void pulsante_eliminaColonna();
    void modificaCella();
};

#endif // MAINWINDOW_H
