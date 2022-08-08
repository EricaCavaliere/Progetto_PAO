#ifndef GRAFICOLINEA_H
#define GRAFICOLINEA_H

#include <QWidget>
#include <QtCharts>

typedef QPair<QPointF, QString> Punto;
typedef QVector<Punto> DatiLinea;
typedef QVector<DatiLinea> DatiTabella;

class GraficoLinea: public QWidget
{
    Q_OBJECT
public:
    GraficoLinea(unsigned int=5,int=100,unsigned int=1);
    virtual ~GraficoLinea();

    DatiTabella generateRandomData(int,int,int)const;
private:
    int nPunti;
    int valoreMax;
    int nLinee;
    DatiTabella dTabella;
    QChartView* grafico;
};

#endif // GRAFICOLINEA_H
