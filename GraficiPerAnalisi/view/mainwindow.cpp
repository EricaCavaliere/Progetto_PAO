#include "mainwindow.h"

#include <QMenuBar>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QPushButton>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QLineSeries>
#include <QScatterSeries>
#include <QPieSeries>
#include <QTime>
#include <QPair>
#include <QCoreApplication>
#include <QInputDialog>


MainWindow::MainWindow(QWidget *parent):QMainWindow(parent){
    setMinimumSize(800,500);
    QMenuBar* menu = new QMenuBar(this);
    impostaMenuBar(menu);
    setMenuBar(menu);

    tab = new QTabWidget(this);
    tab->setMovable(true);
    setCentralWidget(tab);

    status = new QStatusBar();
    setStatusBar(status);
}

MainWindow::MainWindow(DatiGrafico* d,QWidget *parent):QMainWindow(parent){
    window.push_back(d);

    setMinimumSize(800,500);
    QMenuBar* menu = new QMenuBar(this);
    impostaMenuBar(menu);
    setMenuBar(menu);

    tab = new QTabWidget(this);
    tab->addTab(impostaGrafico(d,tab),d->getTitolo());
    tab->setMovable(true);
    setCentralWidget(tab);

    status = new QStatusBar();
    setStatusBar(status);
}

MainWindow::~MainWindow(){}

void MainWindow::aggiungiGrafico(DatiGrafico *d){
    window.push_back(d);
    tab->addTab(impostaGrafico(d,tab),d->getTitolo());
    status->showMessage(tr("Aggiunto il grafico: ")+d->getTitolo());
}

void MainWindow::impostaMenuBar(QMenuBar *menu){
    QMenu* file = new QMenu("File",menu);
    QAction* nuovo = new QAction("Nuovo",file);
    QAction* apri = new QAction("Apri",file);
    QAction* salva = new QAction("Salva",file);
    QAction* salvaTutto = new QAction("Salva tutto",file);
    QAction* chiudi = new QAction("Chiudi",file);
    menu->addMenu(file);
    file->addAction(nuovo);
    file->addAction(apri);
    file->addSeparator();
    file->addAction(salva);
    file->addAction(salvaTutto);
    file->addSeparator();
    file->addAction(chiudi);
    connect(chiudi,&QAction::triggered,qApp,&QCoreApplication::quit);

    QMenu* finestra = new QMenu("Finestra",menu);
    QAction* destra = new QAction("Legenda a destra",finestra);
    QAction* sinistra = new QAction("Legenda a sinistra", finestra);
    QAction* sopra = new QAction("Legenda sopra",finestra);
    QAction* sotto = new QAction("Legenda sotto",finestra);
    animazione = new QAction("Animazione",finestra);
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
    legenda = new QActionGroup(finestra);
    legenda->addAction(destra);
    legenda->addAction(sinistra);
    legenda->addAction(sopra);
    legenda->addAction(sotto);
    sopra->setChecked(true);
    connect(legenda,&QActionGroup::triggered,this,&MainWindow::updateUI);
    connect(animazione,&QAction::triggered,this,&MainWindow::updateUI);

    QMenu* altro = new QMenu("Altro",menu);
    QAction* impostazioni = new QAction("Impostazioni",altro);
    QAction* info = new QAction("Info",altro);
    menu->addMenu(altro);
    altro->addAction(impostazioni);
    altro->addAction(info);
}

QSplitter* MainWindow::impostaGrafico(DatiGrafico* d, QWidget* parent){
    QSplitter *s = new QSplitter;
    s->addWidget(baseTabella(d,parent));

    if(d->getTipo()==DatiGrafico::TipoGrafico::linea)
        s->addWidget(creaGraficoLinea(d));
    else if(d->getTipo()==DatiGrafico::TipoGrafico::punti)
        s->addWidget(creaGraficoPunti(d));
    else if(d->getTipo()==DatiGrafico::TipoGrafico::torta)
        s->addWidget(creaGraficoTorta(d));

    s->setStretchFactor(1, 1);

    return s;
}

QWidget* MainWindow::baseTabella(DatiGrafico* d,QWidget* parent){
    QWidget* obj = new QWidget(parent);
    QVBoxLayout *layoutObj = new QVBoxLayout(obj);

    QWidget* option = new QWidget(obj);
    QGridLayout *layoutOption = new QGridLayout(option);
    QPushButton *addColonna = new QPushButton(tr("Aggiungi colonna"),option);
    QPushButton *addRiga = new QPushButton(tr("Aggiungi riga"),option);
    QPushButton *delColonna = new QPushButton(tr("Elimina colonna"),option);
    QPushButton *delRiga = new QPushButton(tr("Elimina Riga"),option);
    layoutOption->addWidget(addColonna,0,0);
    layoutOption->addWidget(addRiga,1,0);
    layoutOption->addWidget(delColonna,0,1);
    layoutOption->addWidget(delRiga,1,1);
    option->setLayout(layoutOption);

    layoutObj->addWidget(option);
    layoutObj->addWidget(creaTabella(d,obj));
    obj->setLayout(layoutObj);
    return obj;
}

QTableView* MainWindow::creaTabella(DatiGrafico* d, QWidget *parent){
    //con insert si possono inserire nuove righe o colonne.
    QAbstractItemModel *model;
    if(d->count()>0)
        model = new QStandardItemModel(d->getLinea(0).count(),d->count()*2,parent);
    else
        model = new QStandardItemModel(0,d->count()*2);

    if(d->getTipo()==DatiGrafico::TipoGrafico::linea || d->getTipo()==DatiGrafico::TipoGrafico::punti){
        QString nome;
        for(int i=0;i<d->count();++i){
            nome = tr("x - serie ")+QString::number(i);
            model->setHeaderData(i*2,Qt::Horizontal,nome);
            nome = tr("y - serie ")+QString::number(i);
            model->setHeaderData(i*2+1,Qt::Horizontal,nome);
        }
        if(d->count()>0){
            QModelIndex index;
            QVariant value;
            for(int i=0;i<d->getLinea(0).count();++i){
                for(int j=0;j<d->count();++j){
                    //x
                    index = model->index(i,j*2);
                    value = d->getPunto(j,i).first.x();
                    model->setData(index,value);
                    //y
                    index = model->index(i,j*2+1);
                    value = d->getPunto(j,i).first.y();
                    model->setData(index,value);
                }
            }
        }
    }else if(d->getTipo()==DatiGrafico::TipoGrafico::torta){
        QString nome;
        for(int i=0;i<d->count();++i){
            nome = tr("Torta ")+QString::number(i)+tr(" - nome fette");
            model->setHeaderData(i*2,Qt::Horizontal,nome);
            nome = tr("Torta ")+QString::number(i)+tr(" - dati fette");
            model->setHeaderData(i*2+1,Qt::Horizontal,nome);
        }
        if(d->count()>0){
            QModelIndex index;
            QVariant value;
            for(int i=0;i<d->getLinea(0).count();++i){
                for(int j=0;j<d->count();++j){
                    //nome
                    index = model->index(i,j*2);
                    value = d->getPunto(j,i).second;
                    model->setData(index,value);
                    //dato (y)
                    index = model->index(i,j*2+1);
                    value = d->getPunto(j,i).first.y();
                    model->setData(index,value);
                }
            }
        }
    }

    QTableView* view = new QTableView;
    view->setModel(model);
    QItemSelectionModel *selectionModel = new QItemSelectionModel(model);
    view->setSelectionModel(selectionModel);
    QHeaderView *headerView = view->horizontalHeader();
    headerView->setStretchLastSection(true);
    tabelle.push_back(view);
    return view;
}


QtCharts::QChartView* MainWindow::creaGraficoLinea(DatiGrafico* d){
    QtCharts::QChart *chart = new QtCharts::QChart();
    chart->setTitle(d->getTitolo());

    QString name("Series ");
    int nameIndex = 0;
    /*
    for(DatiTabella::Iterator i=d->getTabella().begin();i!=d->getTabella().end();++i){
        QtCharts::QLineSeries *series = new QtCharts::QLineSeries(chart);
        for(DatiLinea::Iterator j = i->begin();j!=i->end();++j){
            series->append(j->first);
        }
        series->setName(name + QString::number(nameIndex));
        nameIndex++;
        chart->addSeries(series);
    }
    */
    for(const DatiLinea &linea : d->getTabella()){
        QtCharts::QLineSeries *series = new QtCharts::QLineSeries(chart);
        for(const DatiPunto &punto : linea)
            series->append(punto.first);
        series->setName(name + QString::number(nameIndex));
        nameIndex++;
        chart->addSeries(series);
    }
    chart->createDefaultAxes();
    QtCharts::QChartView* view = new QtCharts::QChartView(chart);
    view->setRenderHints(QPainter::Antialiasing);
    grafici.push_back(view);
    return view;
}

QtCharts::QChartView* MainWindow::creaGraficoPunti(DatiGrafico* d){
    // scatter chart
    QtCharts::QChart *chart = new QtCharts::QChart();
    chart->setTitle(d->getTitolo());
    QString name("Series ");
    int nameIndex = 0;
    for (const DatiLinea &list : d->getTabella()) {
        QtCharts::QScatterSeries *series = new QtCharts::QScatterSeries(chart);
        for (const DatiPunto &data : list)
            series->append(data.first);
        series->setName(name + QString::number(nameIndex));
        nameIndex++;
        chart->addSeries(series);
    }
    chart->createDefaultAxes();
    QtCharts::QChartView* view = new QtCharts::QChartView(chart);
    view->setRenderHints(QPainter::Antialiasing);
    grafici.push_back(view);
    return view;
}

QtCharts::QChartView* MainWindow::creaGraficoTorta(DatiGrafico* d){
    QtCharts::QChart *chart = new QtCharts::QChart();
    chart->setTitle(d->getTitolo());

    qreal pieSize = 1.0 / d->count();
    for (int i = 0; i < d->count(); i++) {
        QtCharts::QPieSeries *series = new QtCharts::QPieSeries(chart);
        for (const DatiPunto &data : d->getTabella()[i]) {
            QtCharts::QPieSlice *slice = series->append(data.second, data.first.y());
            slice->setLabelVisible();
        }
        //serve per posizionare in modo corretto i grafici a torta
        qreal hPos = (pieSize / 2) + (i / (qreal) d->count());
        series->setPieSize(pieSize);
        series->setHorizontalPosition(hPos);
        series->setVerticalPosition(0.5);

        chart->addSeries(series);
    }
    QtCharts::QChartView* view = new QtCharts::QChartView(chart);
    view->setRenderHints(QPainter::Antialiasing);
    grafici.push_back(view);
    return view;
}

//DA COMPLETARE ------------------------------------------------------------------
void MainWindow::menu_file_nuovo(){}
//DA COMPLETARE ------------------------------------------------------------------
void MainWindow::menu_file_apri(){}

//DA COMPLETARE ------------------------------------------------------------------
void MainWindow::menu_file_salva(){}
//DA COMPLETARE ------------------------------------------------------------------
void MainWindow::menu_file_salvaTutto(){}

//DA COMPLETARE ------------------------------------------------------------------
void MainWindow::updateUI(){
    QAction* check = legenda->checkedAction();
    if(check->text()=="Legenda sopra"){
        for (QtCharts::QChartView *chartView : grafici) {
            chartView->chart()->legend()->setAlignment(Qt::AlignTop);
            chartView->chart()->legend()->show();
        }
    }else if(check->text()=="Legenda sotto"){
        for (QtCharts::QChartView *chartView : grafici) {
            chartView->chart()->legend()->setAlignment(Qt::AlignBottom);
            chartView->chart()->legend()->show();
        }
    }else if(check->text()=="Legenda a destra"){
        for (QtCharts::QChartView *chartView : grafici) {
            chartView->chart()->legend()->setAlignment(Qt::AlignRight);
            chartView->chart()->legend()->show();
        }
    }else if(check->text()=="Legenda a sinistra"){
        for (QtCharts::QChartView *chartView : grafici) {
            chartView->chart()->legend()->setAlignment(Qt::AlignLeft);
            chartView->chart()->legend()->show();
        }
    }

    if(animazione->isChecked()){
        for(QtCharts::QChartView *chartview : grafici) {
            chartview->chart()->setAnimationOptions(QtCharts::QChart::AllAnimations);
        }
    }else{
        for(QtCharts::QChartView *chartview : grafici) {
            chartview->chart()->setAnimationOptions(QtCharts::QChart::NoAnimation);
        }
    }
}

//DA COMPLETARE ------------------------------------------------------------------
void MainWindow::menu_altro_impostazioni(){}
//DA COMPLETARE ------------------------------------------------------------------
void MainWindow::menu_altro_info(){}
//DA COMPLETARE ------------------------------------------------------------------
void MainWindow::pulsante_aggiungiColonna(){

}
//DA COMPLETARE ------------------------------------------------------------------
void MainWindow::pulsante_aggiungiRiga(){}
//DA COMPLETARE ------------------------------------------------------------------
void MainWindow::pulsante_eliminaColonna(){}
//DA COMPLETARE ------------------------------------------------------------------
void MainWindow::pulsante_eliminaRiga(){}
//DA COMPLETARE ------------------------------------------------------------------
void MainWindow::modificaCella(){}























