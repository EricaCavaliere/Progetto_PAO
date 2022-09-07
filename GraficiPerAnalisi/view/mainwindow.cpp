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
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent){
    setMinimumSize(800,500);
    QMenuBar* menu = new QMenuBar(this);
    impostaMenuBar(menu);
    setMenuBar(menu);

    tab = new QTabWidget(this);
    tab->setMovable(true);
    setCentralWidget(tab);
}

MainWindow::~MainWindow(){}

void MainWindow::aggiungiGrafico(const DatiGrafico &d){
    DatiGrafico dati(d);
    window.push_back(dati);
    tab->addTab(impostaGrafico(d,tab),d.getIntestazione());
}

void MainWindow::impostaMenuBar(QMenuBar *menu){
    QMenu* file = new QMenu("File",menu);
    QAction* nuovo = new QAction("Nuovo",file);
    QAction* apri = new QAction("Apri",file);
    QAction* salva = new QAction("Salva",file);
    QAction* rinomina = new QAction("Rinomina",file);
    QAction* chiudi = new QAction("Chiudi",file);
    menu->addMenu(file);
    file->addAction(nuovo);
    file->addAction(apri);
    file->addAction(salva);
    file->addSeparator();
    file->addAction(rinomina);
    file->addSeparator();
    file->addAction(chiudi);
    connect(nuovo,&QAction::triggered,this,&MainWindow::menu_file_nuovo);
    connect(apri,&QAction::triggered,this,&MainWindow::menu_file_apri);
    connect(salva,&QAction::triggered,this,&MainWindow::menu_file_salva);
    connect(rinomina,&QAction::triggered,this,&MainWindow::menu_file_rinomina);
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
    connect(animazione,&QAction::triggered,this,&MainWindow::updateUIanimation);
}

QSplitter* MainWindow::impostaGrafico(const DatiGrafico &d, QWidget* parent){
    QSplitter *s = new QSplitter;
    s->addWidget(baseTabella(d,parent));
    if(d.getTipo()==DatiGrafico::TipoGrafico::linea)
        s->addWidget(creaGraficoLinea(d));
    else if(d.getTipo()==DatiGrafico::TipoGrafico::punti)
        s->addWidget(creaGraficoPunti(d));
    else if(d.getTipo()==DatiGrafico::TipoGrafico::torta)
        s->addWidget(creaGraficoTorta(d));

    s->setStretchFactor(1, 1);

    return s;
}

QWidget* MainWindow::baseTabella(const DatiGrafico &d, QWidget* parent){
    QWidget* obj = new QWidget(parent);
    QVBoxLayout *layoutObj = new QVBoxLayout(obj);

    QWidget* option = new QWidget(obj);
    QGridLayout *layoutOption = new QGridLayout(option);
    QPushButton *addColonna = new QPushButton(tr("Aggiungi colonna"),option);
    QPushButton *delColonna = new QPushButton(tr("Elimina colonna"),option);
    QPushButton *addRiga = new QPushButton(tr("Aggiungi riga"),option);
    QPushButton *delRiga = new QPushButton(tr("Elimina riga"),option);
    layoutOption->addWidget(addColonna,0,0);
    layoutOption->addWidget(delColonna,0,1);
    layoutOption->addWidget(addRiga,1,0);
    layoutOption->addWidget(delRiga,1,1);
    option->setLayout(layoutOption);

    connect(addColonna,&QPushButton::clicked,this,&MainWindow::pulsante_aggiungiColonna);
    connect(delColonna,&QPushButton::clicked,this,&MainWindow::pulsante_eliminaColonna);
    connect(addRiga,&QPushButton::clicked,this,&MainWindow::pulsante_aggiungiRiga);
    connect(delRiga,&QPushButton::clicked,this,&MainWindow::pulsante_eliminaRiga);

    layoutObj->addWidget(option);
    layoutObj->addWidget(creaTabella(d,obj));
    obj->setLayout(layoutObj);
    return obj;
}

QTableView* MainWindow::creaTabella(const DatiGrafico &d, QWidget *parent){
    QAbstractItemModel *model;
    if(d.tabella.count()>0)
        model = new QStandardItemModel(d.tabella.at(0).count(),d.tabella.count()*2,parent);
    else
        model = new QStandardItemModel(0,d.tabella.count()*2);

    if(d.getTipo()==DatiGrafico::TipoGrafico::linea || d.getTipo()==DatiGrafico::TipoGrafico::punti){
        QString nome;
        for(int i=0;i<d.tabella.count();++i){
            nome = tr("x - Serie")+QString::number(i+1);
            model->setHeaderData(i*2,Qt::Horizontal,nome);
            nome = tr("y - Serie")+QString::number(i+1);
            model->setHeaderData(i*2+1,Qt::Horizontal,nome);
        }
        if(d.tabella.count()>0){
            QModelIndex index;
            QVariant value;
            for(int i=0;i<d.tabella.at(0).count();++i){
                for(int j=0;j<d.tabella.count();++j){
                    index = model->index(i,j*2);
                    value = d.tabella.at(j).at(i).first.x();
                    model->setData(index,value);
                    index = model->index(i,j*2+1);
                    value = d.tabella.at(j).at(i).first.y();
                    model->setData(index,value);
                }
            }
        }
    }else if(d.getTipo()==DatiGrafico::TipoGrafico::torta){
        QString nome;
        for(int i=0;i<d.tabella.count();++i){
            nome = tr("Torta ")+QString::number(i+1)+tr(" - fette");
            model->setHeaderData(i*2,Qt::Horizontal,nome);
            nome = tr("Torta ")+QString::number(i+1)+tr(" - dati");
            model->setHeaderData(i*2+1,Qt::Horizontal,nome);
        }
        if(d.tabella.count()>0){
            QModelIndex index;
            QVariant value;
            for(int i=0;i<d.tabella.at(0).count();++i){
                for(int j=0;j<d.tabella.count();++j){
                    index = model->index(i,j*2);
                    value = d.tabella.at(j).at(i).second;
                    model->setData(index,value);
                    index = model->index(i,j*2+1);
                    value = d.tabella.at(j).at(i).first.y();
                    model->setData(index,value);
                }
            }
        }
    }
    QTableView* view = new QTableView;
    view->setModel(model);
    QItemSelectionModel *selectionModel = new QItemSelectionModel(model);
    view->setSelectionModel(selectionModel);
    connect(view->model(),&QAbstractItemModel::dataChanged,this,&MainWindow::modificaCella);
    tabelle.push_back(view);
    return view;
}


QtCharts::QChartView* MainWindow::creaGraficoLinea(const DatiGrafico &d){
    QtCharts::QChart *chart = new QtCharts::QChart();
    chart->setTitle(d.getTitolo());
    int nameIndex = 1;
    for(const DatiLinea &linea : d.tabella){
        QtCharts::QLineSeries *series = new QtCharts::QLineSeries(chart);
        for(const DatiPunto &punto : linea)
            series->append(punto.first);
        series->setName("Serie " + QString::number(nameIndex));
        nameIndex++;
        chart->addSeries(series);
    }
    chart->createDefaultAxes();
    QtCharts::QChartView* view = new QtCharts::QChartView(chart);
    view->setRenderHints(QPainter::Antialiasing);
    grafici.push_back(view);
    return view;
}

QtCharts::QChartView* MainWindow::creaGraficoPunti(const DatiGrafico &d){
    QtCharts::QChart *chart = new QtCharts::QChart();
    chart->setTitle(d.getTitolo());
    int nameIndex = 1;
    for (const DatiLinea &list : d.tabella) {
        QtCharts::QScatterSeries *series = new QtCharts::QScatterSeries(chart);
        for (const DatiPunto &data : list)
            series->append(data.first);
        series->setName("Serie " + QString::number(nameIndex));
        nameIndex++;
        chart->addSeries(series);
    }
    chart->createDefaultAxes();
    QtCharts::QChartView* view = new QtCharts::QChartView(chart);
    view->setRenderHints(QPainter::Antialiasing);
    grafici.push_back(view);
    return view;
}

QtCharts::QChartView* MainWindow::creaGraficoTorta(const DatiGrafico &d){
    QtCharts::QChart *chart = new QtCharts::QChart();
    chart->setTitle(d.getTitolo());
    qreal pieSize = 1.0 / d.tabella.count();
    for (int i = 0; i < d.tabella.count(); i++) {
        QtCharts::QPieSeries *series = new QtCharts::QPieSeries(chart);
        for (const DatiPunto &data : d.tabella.at(i)) {
            QtCharts::QPieSlice *slice = series->append(data.second, data.first.y());
            slice->setLabelVisible();
        }
        qreal hPos = (pieSize / 2) + (i / (qreal) d.tabella.count());
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

QtCharts::QChart* MainWindow::aggiornaGraficoLinea(int index){
    QtCharts::QChart *chart = new QtCharts::QChart();
    chart->setTitle(window.at(index).getTitolo());
    QString name("Series ");
    int nameIndex = 0;
    for(const DatiLinea &linea : window[index].tabella){
        QtCharts::QLineSeries *series = new QtCharts::QLineSeries(chart);
        for(const DatiPunto &punto : linea)
            series->append(punto.first);
        series->setName(name + QString::number(nameIndex));
        nameIndex++;
        chart->addSeries(series);
    }
    chart->createDefaultAxes();
    return chart;
}
QtCharts::QChart* MainWindow::aggiornaGraficoPunti(int index){
    QtCharts::QChart *chart = new QtCharts::QChart();
    chart->setTitle(window.at(index).getTitolo());
    QString name("Series ");
    int nameIndex = 0;
    for (const DatiLinea &list : window[index].tabella) {
        QtCharts::QScatterSeries *series = new QtCharts::QScatterSeries(chart);
        for (const DatiPunto &data : list)
            series->append(data.first);
        series->setName(name + QString::number(nameIndex));
        nameIndex++;
        chart->addSeries(series);
    }
    chart->createDefaultAxes();
    return chart;
}
QtCharts::QChart* MainWindow::aggiornaGraficoTorta(int index){
    QtCharts::QChart *chart = new QtCharts::QChart();
    chart->setTitle(window.at(index).getTitolo());
    qreal pieSize = 1.0 / window[index].tabella.count();
    for (int i = 0; i < window[index].tabella.count(); i++) {
        QtCharts::QPieSeries *series = new QtCharts::QPieSeries(chart);
        for (const DatiPunto &data : window[index].tabella.at(i)) {
            QtCharts::QPieSlice *slice = series->append(data.second, data.first.y());
            slice->setLabelVisible();
        }
        qreal hPos = (pieSize / 2) + (i / (qreal) window[index].tabella.count());
        series->setPieSize(pieSize);
        series->setHorizontalPosition(hPos);
        series->setVerticalPosition(0.5);
        chart->addSeries(series);
    }
    return chart;
}

void MainWindow::menu_file_nuovo(){
    emit nuovo();
}
void MainWindow::menu_file_apri(){
    emit apri();
}

void MainWindow::menu_file_salva(){
    emit salva(window);
}

void MainWindow::menu_file_rinomina(){
    int index = tab->currentIndex();
    if(index>=0){
        QString s = QInputDialog::getText(this,tr("Rinomina"),tr("Inserire il titolo del grafico"));
        if(!s.isNull() && s!=grafici.at(index)->chart()->title()){
            grafici.at(index)->chart()->setTitle(s);
            window[index].setTitolo(s);
            tab->tabBar()->setTabText(index,window.at(index).getIntestazione()+tr("*"));
        }
    }else{
        QMessageBox::information(this,tr("Rinomina"),tr("Non ci sono grafici da rinominare."));
    }
}

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
}

void MainWindow::updateUIanimation(){
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

void MainWindow::pulsante_aggiungiColonna(){
    int index = tab->currentIndex();
    int count = tabelle.at(index)->model()->columnCount();
    tabelle.at(index)->model()->insertColumn(count);
    tabelle.at(index)->model()->insertColumn(count+1);
    DatiLinea l;
    DatiPunto p;
    QModelIndex modelIndex;
    if(window.at(index).getTipo()==DatiGrafico::TipoGrafico::torta){
        tabelle.at(index)->model()->setHeaderData(count,Qt::Horizontal,"Torta "+QString::number(count/2)+" - fette");
        tabelle.at(index)->model()->setHeaderData(count+1,Qt::Horizontal,"Torta "+QString::number(count/2)+" - dati");
        for(int i=0;i<tabelle.at(index)->model()->rowCount();++i){
            modelIndex = tabelle.at(index)->model()->index(i,count);
            tabelle.at(index)->model()->setData(modelIndex,tr(""));
            p.second = tr("");
            modelIndex = tabelle.at(index)->model()->index(i,count+1);
            tabelle.at(index)->model()->setData(modelIndex,0.0);
            p.first.setY(0.0);
            l.push_back(p);
        }
        window[index].tabella.push_back(l);
        grafici.at(index)->setChart(aggiornaGraficoTorta(index));
    }else{
        tabelle.at(index)->model()->setHeaderData(count,Qt::Horizontal,"x - Serie "+QString::number(count/2));
        tabelle.at(index)->model()->setHeaderData(count+1,Qt::Horizontal,"y - Serie "+QString::number(count/2));
        for(int i=0;i<tabelle.at(index)->model()->rowCount();++i){
            modelIndex = tabelle.at(index)->model()->index(i,count);
            tabelle.at(index)->model()->setData(modelIndex,0.0);
            p.first.setX(0.0);
            modelIndex = tabelle.at(index)->model()->index(i,count+1);
            tabelle.at(index)->model()->setData(modelIndex,0.0);
            p.first.setY(0.0);
            l.push_back(p);
        }
        window[index].tabella.push_back(l);
        if(window.at(index).getTipo()==DatiGrafico::TipoGrafico::linea)
            grafici.at(index)->setChart(aggiornaGraficoLinea(index));
        else if(window.at(index).getTipo()==DatiGrafico::TipoGrafico::punti)
            grafici.at(index)->setChart(aggiornaGraficoPunti(index));
    }
    grafici.at(index)->setRenderHint(QPainter::Antialiasing);
    updateUI();
    updateUIanimation();
    tab->tabBar()->setTabText(index,window.at(index).getIntestazione()+tr("*"));
}

void MainWindow::pulsante_eliminaColonna(){
    int index = tab->currentIndex();
    int count = tabelle.at(index)->model()->columnCount();
    if(count>0){
        tabelle.at(index)->model()->removeColumn(count-1);
        tabelle.at(index)->model()->removeColumn(count-2);
        window[index].tabella.pop_back();
        if(window.at(index).getTipo()==DatiGrafico::TipoGrafico::torta)
            grafici.at(index)->setChart(aggiornaGraficoTorta(index));
        else if(window.at(index).getTipo()==DatiGrafico::TipoGrafico::linea)
            grafici.at(index)->setChart(aggiornaGraficoLinea(index));
        else if(window.at(index).getTipo()==DatiGrafico::TipoGrafico::punti)
            grafici.at(index)->setChart(aggiornaGraficoPunti(index));
        grafici.at(index)->setRenderHint(QPainter::Antialiasing);
        updateUI();
        updateUIanimation();
        tab->tabBar()->setTabText(index,window.at(index).getIntestazione()+tr("*"));
    }
}

void MainWindow::pulsante_aggiungiRiga(){
    int index = tab->currentIndex();
    int count = tabelle.at(index)->model()->rowCount();
    tabelle.at(index)->model()->insertRow(count);
    DatiPunto p;
    QModelIndex modelIndex;
    if(window.at(index).getTipo()==DatiGrafico::TipoGrafico::torta){
        for(int i=0;i<tabelle.at(index)->model()->columnCount();i+=2){
            modelIndex = tabelle.at(index)->model()->index(count,i);
            tabelle.at(index)->model()->setData(modelIndex,tr(""));
            p.second = tr("");
            modelIndex = tabelle.at(index)->model()->index(count,i+1);
            tabelle.at(index)->model()->setData(modelIndex,0.0);
            p.first.setY(0.0);
            window[index].tabella[i/2].push_back(p);
        }
        grafici.at(index)->setChart(aggiornaGraficoTorta(index));
    }else{
        for(int i=0;i<tabelle.at(index)->model()->columnCount();i+=2){
            modelIndex = tabelle.at(index)->model()->index(count,i);
            tabelle.at(index)->model()->setData(modelIndex,0.0);
            p.first.setX(0.0);
            modelIndex = tabelle.at(index)->model()->index(count,i+1);
            tabelle.at(index)->model()->setData(modelIndex,0.0);
            p.first.setY(0.0);
            window[index].tabella[i/2].push_back(p);
        }
        if(window.at(index).getTipo()==DatiGrafico::TipoGrafico::linea)
            grafici.at(index)->setChart(aggiornaGraficoLinea(index));
        else if(window.at(index).getTipo()==DatiGrafico::TipoGrafico::punti)
            grafici.at(index)->setChart(aggiornaGraficoPunti(index));
    }
    grafici.at(index)->setRenderHint(QPainter::Antialiasing);
    updateUI();
    updateUIanimation();
    tab->tabBar()->setTabText(index,window.at(index).getIntestazione()+tr("*"));
}

void MainWindow::pulsante_eliminaRiga(){
    int index = tab->currentIndex();
    int count = tabelle.at(index)->model()->rowCount();
    if(count>0){
        tabelle.at(index)->model()->removeRow(count-1);
        for(int i=0;i<window.at(index).tabella.count();++i)
            window[index].tabella[i].pop_back();
        if(window.at(index).getTipo()==DatiGrafico::TipoGrafico::torta)
            grafici.at(index)->setChart(aggiornaGraficoTorta(index));
        else if(window.at(index).getTipo()==DatiGrafico::TipoGrafico::linea)
            grafici.at(index)->setChart(aggiornaGraficoLinea(index));
        else if(window.at(index).getTipo()==DatiGrafico::TipoGrafico::punti)
            grafici.at(index)->setChart(aggiornaGraficoPunti(index));
        grafici.at(index)->setRenderHint(QPainter::Antialiasing);
        updateUI();
        updateUIanimation();
        tab->tabBar()->setTabText(index,window.at(index).getIntestazione()+tr("*"));
    }
}

void MainWindow::modificaCella(){
    int index = tab->currentIndex();
    if(window.at(index).getTipo()==DatiGrafico::TipoGrafico::torta){
        QModelIndex modelIndex;
        for(int j=0;j<window[index].tabella.count();++j){
            for(int i=0;i<window[index].tabella.at(j).count();++i){
                modelIndex = tabelle.at(index)->model()->index(i,j*2);
                window[index].tabella[j][i].second = modelIndex.data().toString();
                modelIndex = tabelle.at(index)->model()->index(i,j*2+1);
                window[index].tabella[j][i].first.setY(modelIndex.data().toInt());
            }
        }
        grafici.at(index)->setChart(aggiornaGraficoTorta(index));
    }else{
        QModelIndex modelIndexX,modelIndexY;
        for(int j=0;j<window[index].tabella.count();++j){
            for(int i=0;i<window[index].tabella.at(j).count();++i){
                modelIndexX = tabelle.at(index)->model()->index(i,j*2);
                modelIndexY = tabelle.at(index)->model()->index(i,j*2+1);
                window[index].tabella[j][i].first.setX(modelIndexX.data().toInt());
                window[index].tabella[j][i].first.setY(modelIndexY.data().toInt());
            }
        }
        if(window.at(index).getTipo()==DatiGrafico::TipoGrafico::linea)
            grafici.at(index)->setChart(aggiornaGraficoLinea(index));
        else if(window.at(index).getTipo()==DatiGrafico::TipoGrafico::punti)
            grafici.at(index)->setChart(aggiornaGraficoPunti(index));
    }
    grafici.at(index)->setRenderHint(QPainter::Antialiasing);
    updateUI();
    updateUIanimation();
    tab->tabBar()->setTabText(index,window.at(index).getIntestazione()+tr("*"));
}
