#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QPieSeries>
#include <QtCharts>

typedef QPair<QPointF, QString> Punto;
typedef QVector<Punto> DatiLinea;
typedef QVector<DatiLinea> DatiTabella;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
/*
private slots:
    void menu_file_nuovo();
    void menu_file_apri();
    void menu_file_slava();
    void menu_file_salvaTutto();
    void menu_file_chiudi();
    void menu_finestra_legenda(int);
    void menu_finestra_animazione();
    void menu_altro_impostazioni();
    void menu_altro_info();
    void tab_close();
    void pulsante_aggiungiColonna();
    void pulsante_aggiungiRiga();
    void pulsante_eliminaColonna();
    void pulsante_eliminaRiga();
    void modificaCella();
*/
private:
    void impostaMenuBar(QMenuBar*)const;
    QSplitter* impostaGrafico(QWidget* = 0,unsigned int=0)const;
    QWidget* baseTabella(QWidget* = 0,unsigned int=0) const;
    QWidget* impostaOpzioniTabella(QWidget* = 0)const;
    QTableView* creaTabella(unsigned int=0)const;
    QChartView* creaGraficoLinea()const;
    QChartView* creaGraficoPunti()const;
    QChartView* creaGraficoTorta()const;

    DatiTabella generateRandomData(int,int,int)const;

    int nPunti;
    int valoreMax;
    unsigned int nLinee;
    DatiTabella tabella;

};

#endif // MAINWINDOW_H
