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
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QChart* creaGraficoLinea()const;
    QChart* creaGraficoTorta()const;
    QChart* creaGraficoPunti()const;
    DatiTabella generateRandomData(int,int,int)const;

private:
    int nPunti;
    int valoreMax;
    int nLinee;
    DatiTabella tabella;

};

#endif // MAINWINDOW_H
