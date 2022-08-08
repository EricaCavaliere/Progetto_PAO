#include "graficolinea.h"

GraficoLinea::GraficoLinea(QWidget* parent, unsigned int p,int m,unsigned int l)
    :QWidget(parent), nPunti(p),valoreMax(m),nLinee(l),dTabella(generateRandomData(l,m,p)){

    QChart *chart = new QChart();
    chart->setTitle("Grafico a Linea");

    QString name("Series ");
    int nameIndex = 0;
    for(const DatiLinea &linea : dTabella){
        QLineSeries *series = new QLineSeries(chart);
        for(const Punto &punto : linea)
            series->append(punto.first);
        series->setName(name + QString::number(nameIndex));
        nameIndex++;
        chart->addSeries(series);
    }

}

DatiTabella GraficoLinea::generateRandomData(int listCount, int valueMax, int valueCount) const
{
    DatiTabella dataTable;

    // set seed for random stuff
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

    // generate random data
    for (int i(0); i < listCount; i++) {
        DatiLinea dataList;
        qreal yValue(0);
        for (int j(0); j < valueCount; j++) {
            yValue = yValue + (qreal)(qrand() % valueMax) / (qreal) valueCount;
            QPointF value((j + (qreal) rand() / (qreal) RAND_MAX) * ((qreal) valoreMax / (qreal) valueCount)
                          ,yValue);
            QString label = "Slice " + QString::number(i) + ":" + QString::number(j);
            dataList << Punto(value, label);
        }
        dataTable << dataList;
    }

    return dataTable;
}
