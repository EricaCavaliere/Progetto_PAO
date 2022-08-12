#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    :QMainWindow(parent),nPunti(10),valoreMax(100),nLinee(2),tabella(generateRandomData(nLinee,valoreMax,nPunti))
{
    setMinimumSize(800,500);
    QMenuBar* menu = new QMenuBar(this);
    impostaMenuBar(menu);
    setMenuBar(menu);

    QTabWidget *tab = new QTabWidget(this);
    tab->addTab(impostaGrafico(tab,0),tr("Grafico Linea"));
    tab->addTab(impostaGrafico(tab,1),tr("Grafico Punti"));
    tab->addTab(impostaGrafico(tab,2),tr("Grafico Torta"));
    tab->setMovable(true);

    //istruzioni per chiudere una scheda tab
    //bisgona vedere come connettere la pagine del grafico con la scheda interessata utilizzando gli slot
    //tab->setTabsClosable(true);
    //connect(tab->tabBar(), &QTabBar::tabCloseRequested, tab->tabBar(), &QTabBar::removeTab);

    setCentralWidget(tab);

    QStatusBar* status = new QStatusBar();
    setStatusBar(status);
}

MainWindow::~MainWindow(){}

void MainWindow::impostaMenuBar(QMenuBar* menu)const
{
    QMenu* file = new QMenu("File",menu);
    QAction* nuovo = new QAction("Nuovo",file);
    QAction* apri = new QAction("Apri",file);
    QAction* salva = new QAction("Salva",file);
    QAction* salvaTutto = new QAction("Salva tutto",file);
    QAction* chiudi = new QAction("Chiudi",file);
    menu->addMenu(file);
    file->addAction(nuovo);
    file->addAction(apri);
    file->addAction(salva);
    file->addAction(salvaTutto);
    file->addSeparator();
    file->addAction(chiudi);

    QMenu* finestra = new QMenu("Finestra",menu);
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
    menu->addMenu(finestra);
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

    QMenu* altro = new QMenu("Altro",menu);
    QAction* impostazioni = new QAction("Impostazioni",altro);
    QAction* info = new QAction("Info",altro);
    menu->addMenu(altro);
    altro->addAction(impostazioni);
    altro->addAction(info);
}

QSplitter* MainWindow::impostaGrafico(QWidget* parent,unsigned int i)const{
    QSplitter *splitter = new QSplitter;
    splitter->addWidget(baseTabella(parent,i));
    if(i==0) splitter->addWidget(creaGraficoLinea());
    else if(i==1) splitter->addWidget(creaGraficoPunti());
    else splitter->addWidget(creaGraficoTorta());
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);
    return splitter;
}

QWidget* MainWindow::baseTabella(QWidget* parent,unsigned int i) const{
    QWidget* obj = new QWidget(parent);
    QVBoxLayout *layout = new QVBoxLayout(obj);
    layout->addWidget(impostaOpzioniTabella(parent));
    layout->addWidget(creaTabella(i));
    obj->setLayout(layout);
    return obj;
}

QWidget* MainWindow::impostaOpzioniTabella(QWidget* parent)const{
    QWidget* obj = new QWidget(parent);
    QGridLayout *layout = new QGridLayout(obj);
    QPushButton *addColonna = new QPushButton(tr("Aggiungi colonna"),obj);
    QPushButton *addRiga = new QPushButton(tr("Aggiungi riga"),obj);
    QPushButton *delColonna = new QPushButton(tr("Elimina colonna"),obj);
    QPushButton *delRiga = new QPushButton(tr("Elimina Riga"),obj);
    layout->addWidget(addColonna,0,0);
    layout->addWidget(addRiga,1,0);
    layout->addWidget(delColonna,0,1);
    layout->addWidget(delRiga,1,1);

    obj->setLayout(layout);
    return obj;
}

QTableView* MainWindow::creaTabella(unsigned int i)const{
    //con insert si possono inserire nuove righe o colonne.
    QAbstractItemModel *model;
    model = new QStandardItemModel(tabella.at(0).count(),tabella.count()*2);
    if(i<2){
        QString nome;
        for(int i=0;i<tabella.count();++i){
            nome = tr("x - serie ")+QString::number(i);
            model->setHeaderData(i*2,Qt::Horizontal,nome);
            nome = tr("y - serie ")+QString::number(i);
            model->setHeaderData(i*2+1,Qt::Horizontal,nome);
        }
        QModelIndex index;
        QVariant value;
        for(int i=0;i<tabella.at(0).count();++i){
            for(int j=0;j<tabella.count();++j){
                //x
                index = model->index(i,j*2);
                value = tabella.at(j).at(i).first.x();
                model->setData(index,value);
                //y
                index = model->index(i,j*2+1);
                value = tabella.at(j).at(i).first.y();
                model->setData(index,value);
            }
        }
    }else{
        QString nome;
        for(int i=0;i<tabella.count();++i){
            nome = tr("Torta ")+QString::number(i)+tr(" - nome fette");
            model->setHeaderData(i*2,Qt::Horizontal,nome);
            nome = tr("Torta ")+QString::number(i)+tr(" - dati fette");
            model->setHeaderData(i*2+1,Qt::Horizontal,nome);
        }
        QModelIndex index;
        QVariant value;
        for(int i=0;i<tabella.at(0).count();++i){
            for(int j=0;j<tabella.count();++j){
                //nome
                index = model->index(i,j*2);
                value = tabella.at(j).at(i).second;
                model->setData(index,value);
                //dato (y)
                index = model->index(i,j*2+1);
                value = tabella.at(j).at(i).first.y();
                model->setData(index,value);
            }
        }
    }

    QTableView* view = new QTableView;
    view->setModel(model);
    QItemSelectionModel *selectionModel = new QItemSelectionModel(model);
    view->setSelectionModel(selectionModel);
    QHeaderView *headerView = view->horizontalHeader();
    headerView->setStretchLastSection(true);
    return view;
}


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

QChartView* MainWindow::creaGraficoPunti() const{
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

QChartView* MainWindow::creaGraficoTorta() const{
    QChart *chart = new QChart();
    chart->setTitle("Grafico a Torta");

    qreal pieSize = 1.0 / tabella.count();
    for (int i = 0; i < tabella.count(); i++) {
        QPieSeries *series = new QPieSeries(chart);
        for (const Punto &data : tabella[i]) {
            QPieSlice *slice = series->append(data.second, data.first.y());
            slice->setLabelVisible();
            //slice->setExploded();
            /*serve  per mettere in risalto una fetta (in questo caso, la prima)
            if (data == tabella[i].first()) {
                slice->setLabelVisible();
                slice->setExploded();
            }*/
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

DatiTabella MainWindow::generateRandomData(int listCount, int valueMax, int valueCount) const{
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
