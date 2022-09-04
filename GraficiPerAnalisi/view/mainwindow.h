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
    ~MainWindow();

    void aggiungiGrafico(const DatiGrafico &);

private:
    void impostaMenuBar(QMenuBar*);
    QSplitter* impostaGrafico(const DatiGrafico&,QWidget*);
    QWidget* baseTabella(const DatiGrafico&,QWidget*);
    QTableView* creaTabella(const DatiGrafico&,QWidget*);
    QtCharts::QChartView* creaGraficoLinea(const DatiGrafico&);
    QtCharts::QChartView* creaGraficoPunti(const DatiGrafico&);
    QtCharts::QChartView* creaGraficoTorta(const DatiGrafico&);
    QtCharts::QChart* aggiornaGraficoLinea(int);
    QtCharts::QChart* aggiornaGraficoPunti(int);
    QtCharts::QChart* aggiornaGraficoTorta(int);

    QActionGroup* legenda;
    QAction* animazione;
    QTabWidget *tab;

    QVector<DatiGrafico> window;
    QVector<QTableView*> tabelle;
    QVector<QtCharts::QChartView*> grafici;

signals:
    void nuovo();
    void apri();
    void salva(const QVector<DatiGrafico>&);
public slots:
    void menu_file_nuovo();
    void menu_file_apri();
    void menu_file_salva();
private slots:
    void menu_file_rinomina();
    void updateUI();
    void updateUIanimation();
    void pulsante_aggiungiColonna();
    void pulsante_eliminaColonna();
    void modificaCella();
};

#endif // MAINWINDOW_H
