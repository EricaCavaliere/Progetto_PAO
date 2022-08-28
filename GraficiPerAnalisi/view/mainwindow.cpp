#include "mainwindow.h"
#include "modificatabella.h"

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

MainWindow::MainWindow(DatiGrafico *d,QWidget *parent):QMainWindow(parent){
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

MainWindow::~MainWindow(){
    for(int i=window.count()-1;i>=0;--i)
        delete window.at(i);
}

void MainWindow::aggiungiGrafico(DatiGrafico* d){
    window.push_back(d);
    tab->addTab(impostaGrafico(d,tab),d->getTitolo());
    status->showMessage(tr("Aggiunto il grafico: ")+d->getTitolo());
}

void MainWindow::impostaMenuBar(QMenuBar *menu){
    QMenu* file = new QMenu("File",menu);
    QAction* nuovo = new QAction("Nuovo",file);
    QAction* apri = new QAction("Apri",file);
    QAction* rinomina = new QAction("Rinomina",file);
    QAction* salva = new QAction("Salva",file);
    QAction* salvaTutto = new QAction("Salva tutto",file);
    QAction* chiudi = new QAction("Chiudi",file);
    menu->addMenu(file);
    file->addAction(nuovo);
    file->addAction(apri);
    file->addSeparator();
    file->addAction(rinomina);
    file->addAction(salva);
    file->addAction(salvaTutto);
    file->addSeparator();
    file->addAction(chiudi);
    connect(nuovo,&QAction::triggered,this,&MainWindow::menu_file_rinomina);
    connect(apri,&QAction::triggered,this,&MainWindow::menu_file_apri);
    connect(rinomina,&QAction::triggered,this,&MainWindow::menu_file_rinomina);
    connect(salva,&QAction::triggered,this,&MainWindow::menu_file_salva);
    connect(salvaTutto,&QAction::triggered,this,&MainWindow::menu_file_salvaTutto);
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
    QPushButton *delColonna = new QPushButton(tr("Elimina colonna"),option);
    layoutOption->addWidget(addColonna,0,0);
    layoutOption->addWidget(delColonna,0,1);
    option->setLayout(layoutOption);

    connect(addColonna,&QPushButton::clicked,this,&MainWindow::pulsante_aggiungiColonna);
    connect(delColonna,&QPushButton::clicked,this,&MainWindow::pulsante_eliminaColonna);

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
    connect(view->model(),&QAbstractItemModel::dataChanged,this,&MainWindow::modificaCella);
    tabelle.push_back(view);
    return view;
}


QtCharts::QChartView* MainWindow::creaGraficoLinea(DatiGrafico* d){
    QtCharts::QChart *chart = new QtCharts::QChart();
    chart->setTitle(d->getTitolo());

    QString name("Series ");
    int nameIndex = 0;
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
        for (const DatiPunto &data : d->getTabella().at(i)) {
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

void MainWindow::menu_file_nuovo(){}
void MainWindow::menu_file_apri(){}
void MainWindow::menu_file_salva(){}
void MainWindow::menu_file_salvaTutto(){}

void MainWindow::menu_file_rinomina(){
    int index = tab->currentIndex();
    if(index>=0){
        QString s = QInputDialog::getText(this,tr("Rinomina"),tr("Inserire il titolo del grafico"));
        if(!s.isNull() && s!=grafici.at(index)->chart()->title()){
            status->showMessage(tr("Il grafico \"")+grafici.at(index)->chart()->title()+tr("\" è stato rinominato in \"")+s+"\"");
            grafici.at(index)->chart()->setTitle(s);
            window.at(index)->setTitolo(s);
            tab->tabBar()->setTabText(index,s+tr("*"));
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
            status->showMessage(tr("Legenda posizionata sopra il grafico"));
        }
    }else if(check->text()=="Legenda sotto"){
        for (QtCharts::QChartView *chartView : grafici) {
            chartView->chart()->legend()->setAlignment(Qt::AlignBottom);
            chartView->chart()->legend()->show();
            status->showMessage(tr("Legenda posizionata sotto il grafico"));
        }
    }else if(check->text()=="Legenda a destra"){
        for (QtCharts::QChartView *chartView : grafici) {
            chartView->chart()->legend()->setAlignment(Qt::AlignRight);
            chartView->chart()->legend()->show();
            status->showMessage(tr("Legenda posizionata a destra"));
        }
    }else if(check->text()=="Legenda a sinistra"){
        for (QtCharts::QChartView *chartView : grafici) {
            chartView->chart()->legend()->setAlignment(Qt::AlignLeft);
            chartView->chart()->legend()->show();
            status->showMessage(tr("Legenda posizionata a sinistra"));
        }
    }
}

void MainWindow::updateUIanimation(){
    if(animazione->isChecked()){
        for(QtCharts::QChartView *chartview : grafici) {
            chartview->chart()->setAnimationOptions(QtCharts::QChart::AllAnimations);
            status->showMessage(tr("Animazione attivata"));
        }
    }else{
        for(QtCharts::QChartView *chartview : grafici) {
            chartview->chart()->setAnimationOptions(QtCharts::QChart::NoAnimation);
            status->showMessage(tr("Animazione disattivata"));
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
    if(window.at(index)->getTipo()==DatiGrafico::TipoGrafico::torta){
        tabelle.at(index)->model()->setHeaderData(count,Qt::Horizontal,"Torta "+QString::number(count/2)+" - nome fette");
        tabelle.at(index)->model()->setHeaderData(count+1,Qt::Horizontal,"Torta "+QString::number(count/2)+" - dati fette");

        for(int i=0;i<tabelle.at(index)->model()->rowCount();++i){
            //nome
            modelIndex = tabelle.at(index)->model()->index(i,count);
            QString s = tr("Slice ")+QString::number(count/2)+":"+QString::number(i);
            tabelle.at(index)->model()->setData(modelIndex,s);
            p.second = s;
            //dato (y)
            modelIndex = tabelle.at(index)->model()->index(i,count+1);
            tabelle.at(index)->model()->setData(modelIndex,0.0);
            p.first.setY(0.0);
            l.push_back(p);
        }
        window.at(index)->addLinea(l);

        QtCharts::QChart *chart = new QtCharts::QChart();
        chart->setTitle(window.at(index)->getTitolo());
        qreal pieSize = 1.0 / window.at(index)->count();
        for (int i = 0; i < window.at(index)->count(); i++) {
            QtCharts::QPieSeries *series = new QtCharts::QPieSeries(chart);
            for (const DatiPunto &data : window.at(index)->getTabella().at(i)) {
                QtCharts::QPieSlice *slice = series->append(data.second, data.first.y());
                slice->setLabelVisible();
            }
            qreal hPos = (pieSize / 2) + (i / (qreal) window.at(index)->count());
            series->setPieSize(pieSize);
            series->setHorizontalPosition(hPos);
            series->setVerticalPosition(0.5);

            chart->addSeries(series);
        }
        grafici.at(index)->setChart(chart);
        grafici.at(index)->setRenderHint(QPainter::Antialiasing);
    }else{
        tabelle.at(index)->model()->setHeaderData(count,Qt::Horizontal,"x - serie "+QString::number(count/2));
        tabelle.at(index)->model()->setHeaderData(count+1,Qt::Horizontal,"y - serie "+QString::number(count/2));

        for(int i=0;i<tabelle.at(index)->model()->rowCount();++i){
            //x
            modelIndex = tabelle.at(index)->model()->index(i,count);
            tabelle.at(index)->model()->setData(modelIndex,0.0);
            p.first.setX(0.0);
            //y
            modelIndex = tabelle.at(index)->model()->index(i,count+1);
            tabelle.at(index)->model()->setData(modelIndex,0.0);
            p.first.setY(0.0);
            l.push_back(p);
        }
        window.at(index)->addLinea(l);

        if(window.at(index)->getTipo()==DatiGrafico::TipoGrafico::linea){
            QtCharts::QChart *chart = new QtCharts::QChart();
            chart->setTitle(window.at(index)->getTitolo());
            QString name("Series ");
            int nameIndex = 0;
            for(const DatiLinea &linea : window.at(index)->getTabella()){
                QtCharts::QLineSeries *series = new QtCharts::QLineSeries(chart);
                for(const DatiPunto &punto : linea)
                    series->append(punto.first);
                series->setName(name + QString::number(nameIndex));
                nameIndex++;
                chart->addSeries(series);
            }
            chart->createDefaultAxes();
            grafici.at(index)->setChart(chart);
            grafici.at(index)->setRenderHint(QPainter::Antialiasing);
        }else if(window.at(index)->getTipo()==DatiGrafico::TipoGrafico::punti){
            QtCharts::QChart *chart = new QtCharts::QChart();
            chart->setTitle(window.at(index)->getTitolo());
            QString name("Series ");
            int nameIndex = 0;
            for (const DatiLinea &list : window.at(index)->getTabella()) {
                QtCharts::QScatterSeries *series = new QtCharts::QScatterSeries(chart);
                for (const DatiPunto &data : list)
                    series->append(data.first);
                series->setName(name + QString::number(nameIndex));
                nameIndex++;
                chart->addSeries(series);
            }
            chart->createDefaultAxes();
            grafici.at(index)->setChart(chart);
            grafici.at(index)->setRenderHint(QPainter::Antialiasing);
        }
    }

    grafici.at(index)->setVisible(true);
    tabelle.at(index)->setVisible(true);
    tab->tabBar()->setTabText(index,window.at(index)->getTitolo()+tr("*"));
    status->showMessage(tr("Nel grafico \"")+grafici.at(index)->chart()->title()+tr("\" è stata aggiunta una colonna"));
}

void MainWindow::pulsante_eliminaColonna(){
    int index = tab->currentIndex();
    int count = tabelle.at(index)->model()->columnCount();

    tabelle.at(index)->model()->removeColumn(count-1);
    tabelle.at(index)->model()->removeColumn(count-2);
    window.at(index)->removeLinea();

    if(window.at(index)->getTipo()==DatiGrafico::TipoGrafico::torta){
        QtCharts::QChart *chart = new QtCharts::QChart();
        chart->setTitle(window.at(index)->getTitolo());
        qreal pieSize = 1.0 / window.at(index)->count();
        for (int i = 0; i < window.at(index)->count(); i++) {
            QtCharts::QPieSeries *series = new QtCharts::QPieSeries(chart);
            for (const DatiPunto &data : window.at(index)->getTabella().at(i)) {
                QtCharts::QPieSlice *slice = series->append(data.second, data.first.y());
                slice->setLabelVisible();
            }
            qreal hPos = (pieSize / 2) + (i / (qreal) window.at(index)->count());
            series->setPieSize(pieSize);
            series->setHorizontalPosition(hPos);
            series->setVerticalPosition(0.5);

            chart->addSeries(series);
        }
        grafici.at(index)->setChart(chart);
        grafici.at(index)->setRenderHint(QPainter::Antialiasing);
    }else if(window.at(index)->getTipo()==DatiGrafico::TipoGrafico::linea){
        QtCharts::QChart *chart = new QtCharts::QChart();
        chart->setTitle(window.at(index)->getTitolo());
        QString name("Series ");
        int nameIndex = 0;
        for(const DatiLinea &linea : window.at(index)->getTabella()){
            QtCharts::QLineSeries *series = new QtCharts::QLineSeries(chart);
            for(const DatiPunto &punto : linea)
                series->append(punto.first);
            series->setName(name + QString::number(nameIndex));
            nameIndex++;
            chart->addSeries(series);
        }
        chart->createDefaultAxes();
        grafici.at(index)->setChart(chart);
        grafici.at(index)->setRenderHint(QPainter::Antialiasing);
    }else if(window.at(index)->getTipo()==DatiGrafico::TipoGrafico::punti){
        QtCharts::QChart *chart = new QtCharts::QChart();
        chart->setTitle(window.at(index)->getTitolo());
        QString name("Series ");
        int nameIndex = 0;
        for (const DatiLinea &list : window.at(index)->getTabella()) {
            QtCharts::QScatterSeries *series = new QtCharts::QScatterSeries(chart);
            for (const DatiPunto &data : list)
                series->append(data.first);
            series->setName(name + QString::number(nameIndex));
            nameIndex++;
            chart->addSeries(series);
        }
        chart->createDefaultAxes();
        grafici.at(index)->setChart(chart);
        grafici.at(index)->setRenderHint(QPainter::Antialiasing);
    }

    tab->tabBar()->setTabText(index,window.at(index)->getTitolo()+tr("*"));
    status->showMessage(tr("Nel grafico \"")+grafici.at(index)->chart()->title()+tr("\" è stata eliminata una colonna"));

}

void MainWindow::modificaCella(){
    int index = tab->currentIndex();

    if(window.at(index)->getTipo()==DatiGrafico::TipoGrafico::torta){

        QModelIndex modelIndex;
        for(int i=0;i<window.at(index)->getLinea(0).count();++i){
            for(int j=0;j<window.at(index)->count();++j){
                //nome
                modelIndex = tabelle.at(index)->model()->index(i,j*2);
                window.at(index)->setPunto(modelIndex.data().toString(),j,i);
                //dato (y)
                modelIndex = tabelle.at(index)->model()->index(i,j*2+1);
                window.at(index)->setPunto(0.0,modelIndex.data().toInt(),j,i);
            }
        }

        QtCharts::QChart *chart = new QtCharts::QChart();
        chart->setTitle(window.at(index)->getTitolo());
        qreal pieSize = 1.0 / window.at(index)->count();
        for (int i = 0; i < window.at(index)->count(); i++) {
            QtCharts::QPieSeries *series = new QtCharts::QPieSeries(chart);
            for (const DatiPunto &data : window.at(index)->getTabella().at(i)) {
                QtCharts::QPieSlice *slice = series->append(data.second, data.first.y());
                slice->setLabelVisible();
            }
            //serve per posizionare in modo corretto i grafici a torta
            qreal hPos = (pieSize / 2) + (i / (qreal) window.at(index)->count());
            series->setPieSize(pieSize);
            series->setHorizontalPosition(hPos);
            series->setVerticalPosition(0.5);

            chart->addSeries(series);
        }
        grafici.at(index)->setChart(chart);
        grafici.at(index)->setRenderHint(QPainter::Antialiasing);
    }else{
        QModelIndex modelIndexX,modelIndexY;
        for(int i=0;i<window.at(index)->getLinea(0).count();++i){
            for(int j=0;j<window.at(index)->count();++j){
                //x e y
                modelIndexX = tabelle.at(index)->model()->index(i,j*2);
                modelIndexY = tabelle.at(index)->model()->index(i,j*2+1);
                window.at(index)->setPunto(modelIndexX.data().toInt(),modelIndexY.data().toInt(),j,i);
            }
        }

        if(window.at(index)->getTipo()==DatiGrafico::TipoGrafico::linea){
            QtCharts::QChart *chart = new QtCharts::QChart();
            chart->setTitle(window.at(index)->getTitolo());
            QString name("Series ");
            int nameIndex = 0;
            for(const DatiLinea &linea : window.at(index)->getTabella()){
                QtCharts::QLineSeries *series = new QtCharts::QLineSeries(chart);
                for(const DatiPunto &punto : linea)
                    series->append(punto.first);
                series->setName(name + QString::number(nameIndex));
                nameIndex++;
                chart->addSeries(series);
            }
            chart->createDefaultAxes();
            grafici.at(index)->setChart(chart);
            grafici.at(index)->setRenderHint(QPainter::Antialiasing);
        }else if(window.at(index)->getTipo()==DatiGrafico::TipoGrafico::punti){
            QtCharts::QChart *chart = new QtCharts::QChart();
            chart->setTitle(window.at(index)->getTitolo());
            QString name("Series ");
            int nameIndex = 0;
            for (const DatiLinea &list : window.at(index)->getTabella()) {
                QtCharts::QScatterSeries *series = new QtCharts::QScatterSeries(chart);
                for (const DatiPunto &data : list)
                    series->append(data.first);
                series->setName(name + QString::number(nameIndex));
                nameIndex++;
                chart->addSeries(series);
            }
            chart->createDefaultAxes();
            grafici.at(index)->setChart(chart);
            grafici.at(index)->setRenderHint(QPainter::Antialiasing);
        }
    }
}
