#include "mainwindow.h"
#include "graficolinea.h"


MainWindow::MainWindow(QWidget *parent)
    :QMainWindow(parent),nPunti(10),valoreMax(100),nLinee(2),tabella(generateRandomData(nLinee,valoreMax,nPunti)){

    setMinimumSize(500,500);
    QMenuBar* menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    QMenu* file = new QMenu("File",menuBar);
    QAction* nuovo = new QAction("Nuovo",file);
    QAction* apri = new QAction("Apri",file);
    QAction* salva = new QAction("Salva",file);
    QAction* salvaTutto = new QAction("Salva tutto",file);
    QAction* chiudi = new QAction("Chiudi",file);
    menuBar->addMenu(file);
    file->addAction(nuovo);
    file->addAction(apri);
    file->addAction(salva);
    file->addAction(salvaTutto);
    file->addSeparator();
    file->addAction(chiudi);

    QMenu* grafico = new QMenu("Grafico",menuBar);
    QAction* addLinea = new QAction("Aggiungi Linea",grafico);
    QAction* addPunto = new QAction("Aggiungi punto",grafico);
    QAction* delLinea = new QAction("Elimina Linea",grafico);
    QAction* delPunto = new QAction("Elimina punto");
    menuBar->addMenu(grafico);
    grafico->addSeparator();
    grafico->addAction(addLinea);
    grafico->addAction(addPunto);
    grafico->addAction(delLinea);
    grafico->addAction(delPunto);


    QMenu* finestra = new QMenu("Finestra",menuBar);
    QAction* destra = new QAction("Legenda a destra",finestra);
    QAction* sinistra = new QAction("Legenda a sinistra", finestra);
    QAction* sopra = new QAction("Legenda sopra",finestra);
    QAction* sotto = new QAction("Legenda sotto",finestra);
    QAction* animazione = new QAction("Animazione",finestra);
    destra->setCheckable(true);
    sinistra->setCheckable(true);
    sopra->setCheckable(true);
    sotto->setCheckable(true);
    animazione->setCheckable(true);
    menuBar->addMenu(finestra);
    finestra->addSeparator();
    finestra->addAction(destra);
    finestra->addAction(sinistra);
    finestra->addAction(sopra);
    finestra->addAction(sotto);
    finestra->addSeparator();
    finestra->addAction(animazione);
    QActionGroup* legenda = new QActionGroup(finestra);
    legenda->addAction(destra);
    legenda->addAction(sinistra);
    legenda->addAction(sopra);
    legenda->addAction(sotto);
    sopra->setChecked(true);


    QMenu* altro = new QMenu("Altro",menuBar);
    QAction* impostazioni = new QAction("Impostazioni",altro);
    QAction* info = new QAction("Info",altro);
    menuBar->addMenu(altro);
    altro->addAction(impostazioni);
    altro->addAction(info);


    QTabWidget *tab = new QTabWidget(this);
    tab->addTab(creaGraficoLinea(),tr("Grafico Linea"));
    tab->addTab(creaGraficoTorta(),tr("Grafico Torta"));
    tab->addTab(creaGraficoPunti(),tr("Grafico Punti"));
    tab->setMovable(true);

    //istruzioni per chiudere una scheda tab
    //bisgona vedere come connettere la pagine del grafico con la scheda interessata
    //tab->setTabsClosable(true);
    //connect(tab->tabBar(), &QTabBar::tabCloseRequested, tab->tabBar(), &QTabBar::removeTab);


    setCentralWidget(tab);
}

MainWindow::~MainWindow(){}

QChartView* MainWindow::creaGraficoLinea()const{

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
    QChartView* view = new QChartView(chart);
    view->setRenderHints(QPainter::Antialiasing);
    return view;
}

QChartView* MainWindow::creaGraficoTorta() const
{
    QChart *chart = new QChart();
    chart->setTitle("Grafico a Torta");

    qreal pieSize = 1.0 / tabella.count();
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
        //serve per posizionare in modo corretto i grafici a torta
        qreal hPos = (pieSize / 2) + (i / (qreal) tabella.count());
        series->setPieSize(pieSize);
        series->setHorizontalPosition(hPos);
        series->setVerticalPosition(0.5);

        chart->addSeries(series);
    }
    QChartView* view = new QChartView(chart);
    view->setRenderHints(QPainter::Antialiasing);
    return view;
}

QChartView* MainWindow::creaGraficoPunti() const
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
    QChartView* view = new QChartView(chart);
    view->setRenderHints(QPainter::Antialiasing);
    return view;
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
