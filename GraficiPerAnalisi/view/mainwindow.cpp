#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    :QMainWindow(parent),nPunti(20),valoreMax(100),nLinee(2),tabella(generateRandomData(nLinee,valoreMax,nPunti)){

    setMinimumSize(500,500);

    QMenuBar* menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    QMenu* file = new QMenu("File",menuBar);
    QAction* nuovo = new QAction("Nuovo",file);
    QAction* apri = new QAction("Apri",file);
    QAction* chiudi = new QAction("Chiudi",file);
    menuBar->addMenu(file);
    file->addAction(nuovo);
    file->addAction(apri);
    file->addAction(chiudi);

    QChartView *chartView = new QChartView(creaGraficoPunti());
    setCentralWidget(chartView);
}

MainWindow::~MainWindow(){}

QChart* MainWindow::creaGraficoLinea()const{

    QChart *chart = new QChart();
    chart->setTitle("Grafico a Linea");

    QString name("Series ");
    int nameIndex = 0;
    for(const DatiLinea &linea : tabella){
        QLineSeries *series = new QLineSeries(chart);
        for(const Punto &punto : linea)
            series->append(punto.first);
        series->setName(name + QString::number(nameIndex));
        nameIndex++;
        chart->addSeries(series);
    }
    chart->createDefaultAxes();
    return chart;
}

QChart* MainWindow::creaGraficoTorta() const
{
    QChart *chart = new QChart();
    chart->setTitle("Grafico a Torta");

    for (int i = 0; i < tabella.count(); i++) {
        QPieSeries *series = new QPieSeries(chart);
        for (const Punto &data : tabella[i]) {
            QPieSlice *slice = series->append(data.second, data.first.y());
            //serve  per mettere in risalto una fetta (in questo caso, la prima)
            if (data == tabella[i].first()) {
                slice->setLabelVisible();
                slice->setExploded();
            }
        }
        //qreal hPos = (pieSize / 2) + (i / (qreal) tabella.count());
        //series->setPieSize(pieSize);
        //series->setHorizontalPosition(hPos);
        //series->setVerticalPosition(0.5);
        chart->addSeries(series);
    }

    return chart;
}

QChart* MainWindow::creaGraficoPunti() const
{
    // scatter chart
    QChart *chart = new QChart();
    chart->setTitle("Scatter chart");
    QString name("Series ");
    int nameIndex = 0;
    for (const DatiLinea &list : tabella) {
        QScatterSeries *series = new QScatterSeries(chart);
        for (const Punto &data : list)
            series->append(data.first);
        series->setName(name + QString::number(nameIndex));
        nameIndex++;
        chart->addSeries(series);
    }
    chart->createDefaultAxes();
    return chart;
}

DatiTabella MainWindow::generateRandomData(int listCount, int valueMax, int valueCount) const
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
