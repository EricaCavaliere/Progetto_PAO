#include "controller.h"
#include "model/composto.h"
#include "model/miscela.h"

#include <QString>
#include <QInputDialog>
#include <QDir>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFileDialog>
#include <QJsonArray>
#include <QStandardItemModel>

Controller::Controller(MainWindow* parent):view(parent){
    MainWindow::connect(view,&MainWindow::nuovo,this,&Controller::nuovo_file);
    QObject::connect(view,&MainWindow::nuovo,this,&Controller::nuovo_file);
    MainWindow::connect(view,SIGNAL(MainWindow::nuovo()),this,SLOT(Controller::nuovo_file()));
    QObject::connect(view,SIGNAL(MainWindow::nuovo()),this,SLOT(Controller::nuovo_file()));
}

Controller::~Controller(){}

Elemento Controller::inserire_elemento(){
    bool ok;
    QString nome = QInputDialog::getText(view,QString("Nuovo Elemeneto"),QString("Inserire il nome o la formula dell'elemento:"),QLineEdit::Normal,QString(),&ok);
    if(ok && !nome.isEmpty()){
        int nAtomico = QInputDialog::getInt(view,QString("Nuovo Elemento"),QString("Inserire il numero atomico:"),0,0,200,1,&ok);
        if(ok){
            double nMassa = QInputDialog::getDouble(view,QString("Nuovo Elemento"),QString("Inserire il numero di massa:"),0,0,500,4,&ok);
            if(ok){
                QFile file(QDir::homePath()+"/elementi.json");
                if(!file.open(QIODevice::ReadWrite)){
                    QMessageBox::critical(view,QString("Errore nella lettura del file"),QString("C'è stato un problema di lettura del file \"elementi.json\" per poter inserire il nuovo elemento nella lista."));
                    file.close();
                    return Elemento();
                }
                QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
                QJsonObject element;
                element.insert("nome",nome);
                element.insert("numAtomico",nAtomico);
                element.insert("numMassa",nMassa);
                QJsonObject obj;
                if(doc.isObject()){
                    if(doc.object().contains(QString::number(nAtomico))){
                        QMessageBox::warning(view,QString("Elemento esistente"),
                                             QString("Esiste un elemento con lo stesso numero atomico\nNome: ")+
                                             doc.object()[QString::number(nAtomico)].toObject()["nome"].toString()+
                                             QString("\nNumero atomico: ")+
                                             QString::number(doc.object()[QString::number(nAtomico)].toObject()["numAtomico"].toInt())+
                                             QString("\nNumero di massa: ")+
                                             QString::number(doc.object()[QString::number(nAtomico)].toObject()["numMassa"].toDouble()));
                        file.close();
                        return Elemento();
                    }
                    for(auto i=doc.object().begin();i!=doc.object().end();++i)
                        obj.insert(i.key(),i.value());
                    file.resize(0);
                }
                obj.insert(QString::number(nAtomico),element);
                doc.setObject(obj);
                file.write(doc.toJson());
                file.close();
                return Elemento(nome.toStdString(),nAtomico,nMassa);
            }
        }
    }
    return Elemento();
}

Campione::Stato Controller::lettura_stato(QString stato){
    if(stato==QString("solido"))
        return Campione::solido;
    if(stato==QString("liquido"))
        return Campione::liquido;
    if(stato==QString("gassoso"))
        return Campione::gassoso;
    return Campione::indefinito;
}

Composto Controller::lettura_composto(QFile* file){
    QJsonDocument doc = QJsonDocument::fromJson(file->readAll());
    if(!doc.isObject() || doc.object()["tipologia"]!=QString("composto")){
        QMessageBox::critical(view,QString("Errore nella lettura del file"),QString("C'è stato un problema di lettura del file."));
        return Composto();
    }
    Campione::Stato s = lettura_stato(doc.object()["stato"].toString());
    Composto c(doc.object()["formula"].toString().toStdString(),
            doc.object()["nome"].toString().toStdString(),s,
            doc.object()["massa"].toDouble(),
            doc.object()["volume"].toDouble(),
            doc.object()["temperatura"].toDouble(),
            doc.object()["moli"].toInt());
    QFile elementi(QDir::homePath()+"/elementi.json");
    if(!elementi.open(QIODevice::ReadOnly)){
        QMessageBox::critical(view,QString("Errore nella lettura del file"),QString("C'è stato un problema di lettura del file \"elementi.json\" per poter leggere gli elementi nella lista."));
    }
    QJsonDocument docElementi = QJsonDocument::fromJson(elementi.readAll());
    if(!docElementi.isObject()){
        QMessageBox::critical(view,QString("Errore grave"),QString("Non è stato possibile aprire il Componente indicato."));
    }else{
        QJsonArray lista = doc.object()["lista"].toArray();
        for(int i=0;i<lista.count();++i){
            Elemento e(docElementi.object()[doc.object()["lista"].toArray().at(i).toString()].toObject()["nome"].toString().toStdString(),
                    docElementi.object()[doc.object()["lista"].toArray().at(i).toString()].toObject()["numAtomico"].toInt(),
                    docElementi.object()[doc.object()["lista"].toArray().at(i).toString()].toObject()["numMassa"].toDouble());
            c.push_back(e);
        }
    }
    elementi.close();
    return c;
}

Miscela Controller::lettura_miscela(QFile* file){
    QJsonDocument doc = QJsonDocument::fromJson(file->readAll());
    if(!doc.isObject() || doc.object()["tipologia"]!=QString("composto")){
        QMessageBox::critical(view,QString("Errore nella lettura del file"),QString("C'è stato un problema di lettura del file."));
        return Miscela();
    }
    Campione::Stato s = lettura_stato(doc.object()["stato"].toString());
    Miscela m(doc.object()["nome"].toString().toStdString(),s,
            doc.object()["massa"].toDouble(),
            doc.object()["volume"].toDouble(),
            doc.object()["temperatura"].toDouble(),
            doc.object()["soluzione"].toBool());
    for(auto i = doc.object()["lista"].toObject().begin();i!=doc.object()["lista"].toObject().end();++i){
        s = lettura_stato(i->toObject()["stato"].toString());
        Composto c(i->toObject()["formula"].toString().toStdString(),
                i->toObject()["nome"].toString().toStdString(),s,
                i->toObject()["massa"].toDouble(),
                i->toObject()["volume"].toDouble(),
                i->toObject()["temperatura"].toDouble(),
                i->toObject()["moli"].toInt());
        QFile elementi(QDir::homePath()+"/elementi.json");
        if(!elementi.open(QIODevice::ReadOnly)){
            QMessageBox::critical(view,QString("Errore nella lettura del file"),QString("C'è stato un problema di lettura del file \"elementi.json\" per poter leggere gli elementi nella lista."));
            elementi.close();
            return Miscela();
        }
        QJsonDocument docElementi = QJsonDocument::fromJson(elementi.readAll());
        if(!docElementi.isObject()){
            QMessageBox::critical(view,QString("Errore grave"),QString("Non è stato possibile leggere il Componente indicato."));
            elementi.close();
            return Miscela();
        }
        QJsonArray lista = doc.object()["lista"].toArray();
        for(int i=0;i<lista.count();++i){
            Elemento e(docElementi.object()[doc.object()["lista"].toArray().at(i).toString()].toObject()["nome"].toString().toStdString(),
                    docElementi.object()[doc.object()["lista"].toArray().at(i).toString()].toObject()["numAtomico"].toInt(),
                    docElementi.object()[doc.object()["lista"].toArray().at(i).toString()].toObject()["numMassa"].toDouble());
            c.push_back(e);
        }
        elementi.close();
        m.push_back(c);
    }
    return m;
}

void Controller::nuovo_file(){
    bool ok;
    QStringList items;
    items << QString("Elemento") << QString("Composto") << QString("Miscela") << QString("Altro");
    QString s = QInputDialog::getItem(view,QString("Nuovo grafico"),QString("Che cosa vuole creare?"),items,0,false,&ok);
    if(!ok) return;
    if(s==QString("Elemento")){
        inserire_elemento();
    }else if(s==QString("Composto")){
        bool ok;
        QString formula = QInputDialog::getText(view,QString("Nuovo Composto"),QString("Indicare la formula chimica del composto:"),QLineEdit::Normal,QString(),&ok);
        if(!ok || formula.isEmpty()) return;
        QString nome = QInputDialog::getText(view,QString("Nuovo Composto"),QString("Indicare il nome del composto analizzato:"),QLineEdit::Normal,QString(),&ok);
        if(!ok || nome.isEmpty()) return;
        items.clear();
        items << QString("indefinito") << QString("solido") << QString("liquido") << QString("gassoso");
        QString stato = QInputDialog::getItem(view,QString("Nuovo Composto"),QString("Indicare lo stato del composto:"),items,0,false,&ok);
        double massa=0.0,volume=0.0,temperatura=0.0;
        int moli=0;
        if(ok){
            massa = QInputDialog::getDouble(view,QString("Nuovo Composto"),QString("Inserire la massa:"),0,0,1000000,4,&ok);
            if(ok){
                volume = QInputDialog::getDouble(view,QString("Nuovo Composto"),QString("Inserire il volume:"),0,0,1000000,4,&ok);
                if(ok){
                    temperatura = QInputDialog::getDouble(view,QString("Nuovo Composto"),QString("Inserire la temperatura:"),0,-1000,1000,2,&ok);
                    if(ok)
                        moli = QInputDialog::getInt(view,QString("Nuovo Composto"),QString("Inserire la quantità di moli interessata:"),0,0,1000000);
                }
            }
        }
        Campione::Stato s = lettura_stato(stato);
        Composto composto(formula.toStdString(),nome.toStdString(),s,massa,volume,temperatura,moli);
        QFile file(QDir::homePath()+"/elementi.json");
        if(!file.open(QIODevice::ReadOnly)){
            QMessageBox::critical(view,QString("Errore nella lettura del file"),QString("C'è stato un problema di lettura del file \"elementi.json\" per poter leggere gli elementi nella lista."));
        }else{
            QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
            if(!doc.isObject()){
                ok = QMessageBox::question(view,QString("Nuovo Composto"),QString("Non ci sono elementi da poter inserire, vuole creare un elemento?"));
                while(ok){
                    Elemento e = inserire_elemento();
                    if(e.getNome().empty()){
                        QMessageBox::critical(view,QString("Composto non creato"),QString("Ci sono stati degli errori nella creazione del compposto."));
                        file.close();
                        return;
                    }
                    composto.push_back(e);
                    ok = QMessageBox::question(view,QString("Nuovo Composto"),QString("Vuole creare un altro elemento?"));
                }
            }else{
                items.clear();
                ok = true;
                for(auto i=doc.object().begin();i!=doc.object().end();++i){
                    items << doc.object()[i.key()].toObject()["nome"].toString()+QString("-")+i.key();
                }
                while(!items.empty() && ok){
                    QString n = QInputDialog::getItem(view,QString("Nuovo Composto"),QString("Inserire un elemento.\nCliccare OK per inserire un Elemento nel Composto, altrimenti cliccare Annulla per andare al grafico."),items,0,false,&ok);
                    if(ok){
                        items.removeOne(n);
                        QStringList s;
                        s = n.split("-");
                        composto.push_back(Elemento(doc.object()[s.at(1)].toObject()["nome"].toString().toStdString(),
                                doc.object()[s.at(1)].toObject()["numAtomico"].toInt(),
                                doc.object()[s.at(1)].toObject()["numMassa"].toDouble()));
                    }
                }
            }
        }
        file.close();
        DatiGrafico grafico(QString::fromStdString(composto.getFormulaChimica()),
                      QString::fromStdString(composto.getNome()),
                      DatiGrafico::torta,composto.size());
        DatiPunto punto;
        for(unsigned int i=0; i<composto.size();++i){
            punto.second = QString::fromStdString(composto[i].getNome());
            punto.first.setX(0.0);
            punto.first.setY(1.0);
            grafico.setPunto(punto,0,i);
        }
        Path path;
        path.first = composto;
        path.second = QString();
        model.push_back(path);
        view->aggiungiGrafico(&grafico);
    }else if(s==QString("Miscela")){
        QString nome = QInputDialog::getText(view,QString("Nuova Miscela"),QString("Indicare il nome dela miscela:"),QLineEdit::Normal,QString(),&ok);
        if(!ok || nome.isEmpty()) return;
        items.clear();
        items << QString("indefinito") << QString("solido") << QString("liquido") << QString("gassoso");
        QString stato = QInputDialog::getItem(view,QString("Nuova Miscela"),QString("Indicare lo stato della miscela:"),items,0,false,&ok);
        double massa=0.0,volume=0.0,temperatura=0.0;
        bool soluzione=true;
        if(ok){
            massa = QInputDialog::getDouble(view,QString("Nuova Miscela"),QString("Inserire la massa:"),0,0,1000000,4,&ok);
            if(ok){
                volume = QInputDialog::getDouble(view,QString("Nuova Miscela"),QString("Inserire il volume:"),0,0,1000000,4,&ok);
                if(ok){
                    temperatura = QInputDialog::getDouble(view,QString("Nuova Miscela"),QString("Inserire la temperatura:"),0,-1000,1000,2,&ok);
                    if(ok)
                        soluzione = QMessageBox::question(view,QString("Nuova Miscela"),QString("La Miscela analizzata è una soluzione?"));
                }
            }
        }
        Campione::Stato s = lettura_stato(stato);
        Miscela miscela(nome.toStdString(),s,massa,volume,temperatura,soluzione);
        Composto composto;
        ok=true;
        while(ok){
            QString fileName = QFileDialog::getOpenFileName(view,QString("Nuova Miscela - selezionare Composto"),QDir::homePath(),QString("*.json"));
            if(!fileName.isEmpty()){
                QFile file(fileName);
                if(!file.open(QIODevice::ReadOnly)){
                    QMessageBox::critical(view,QString("Errore nella lettura del file"),QString("C'è stato un problema di lettura del file."));
                }else{
                    composto = lettura_composto(&file);
                    if(!composto.getNome().empty()){
                        miscela.push_back(composto);
                        file.close();
                    }
                }
            }else ok=false;
        }
        unsigned int nmax = 0;
        for(Miscela::iterator i=miscela.begin();i!=miscela.end();++i){
            if(i->size()>nmax) nmax = i->size();
        }
        DatiGrafico grafico(QString::fromStdString(miscela.getNome()),
                            QString::fromStdString(miscela.getNome()),
                            DatiGrafico::torta,nmax,20,miscela.size());
        DatiPunto punto;
        for(unsigned int i=0;i<miscela.size();++i){
            for(unsigned int j=0;j<miscela[i].size();++j){
                punto.second = QString::fromStdString(miscela[i][j].getNome());
                punto.first.setX(0.0);
                punto.first.setY(1.0);
                grafico.setPunto(punto,0,i);
            }
            for(unsigned int j=miscela[i].size();j<nmax;++j){
                punto.second = QString("");
                punto.first.setX(0.0);
                punto.first.setY(0.0);
                grafico.setPunto(punto,0,i);
            }
        }
        Path path;
        path.first = miscela;
        path.second = QString();
        model.push_back(path);
        view->aggiungiGrafico(&grafico);
    }else{
        items.clear();
        items << QString("Analizzare la massa molare di uno o più composti o dei composti di una miscela") //linea
              << QString("Confrontare le temperature dei composti di una miscela") //punti
              << QString("Analizzare la molarità (rapporto moli e volume)");//linea
        s = QInputDialog::getItem(view,QString("Nuovo grafico"),QString("Che cosa vuole creare?"),items,0,false,&ok);
        if(!ok) return;
        if(s=="Analizzare la massa molare di uno o più composti"){
            Composto composto;
            Miscela miscela("Grafico a linea");
            QString fileName = QFileDialog::getOpenFileName(view,QString("Massa molare - selezionare un Composto o una Miscela"),QDir::homePath(),QString("*.json"));
            if(!fileName.isEmpty()){
                QFile file(fileName);
                if(!file.open(QIODevice::ReadOnly)){
                    QMessageBox::critical(view,QString("Errore nella lettura del file"),QString("C'è stato un problema di lettura del file."));
                    file.close();
                }else{
                    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
                    if(!doc.isObject() || !doc.object().contains("tipologia")){
                        QMessageBox::critical(view,QString("Errore nella lettura del file"),QString("C'è stato un problema di lettura del file."));
                    }else{
                        if(doc.object()["tipologia"]=="composto"){
                            do{
                                composto = lettura_composto(&file);
                                if(!composto.getNome().empty()){
                                    miscela.push_back(composto);
                                    file.close();
                                }
                                fileName = QFileDialog::getOpenFileName(view,QString("Massa molare - selezionare un Composto"),QDir::homePath(),QString("*.json"));
                                if(!fileName.isEmpty()){
                                    file.setFileName(fileName);
                                    if(!file.open(QIODevice::ReadOnly)){
                                        QMessageBox::critical(view,QString("Errore nella lettura del file"),QString("C'è stato un problema di lettura del file."));
                                        file.close();
                                        ok = false;
                                    }
                                }
                            }while(ok);
                        }else if(doc.object()["tipologia"]=="miscela"){
                            miscela = lettura_miscela(&file);
                        }
                        if(!miscela.empty()){
                            DatiGrafico grafico(QString::fromStdString(miscela.getNome()),
                                                QString::fromStdString(miscela.getNome()),
                                                DatiGrafico::linea,miscela.size(),20);
                            DatiPunto punto;
                            for(unsigned int i=0;i<miscela.size();++i){
                                punto.second = QString::fromStdString(miscela[i].getNome());
                                punto.first.setX(i+1);
                                punto.first.setY((qreal)miscela[i].getMassaMolare());
                                grafico.setPunto(punto,0,i);
                            }
                            Path path;
                            path.first = miscela;
                            //path.second;
                            //model.append(miscela);
                            view->aggiungiGrafico(&grafico);
                            //aggiungere una finestra a parte per visualizzare la massa molare di ogni composto
                            //controllare se la tabella e' visibile e se le celle sono modificabili
                            //se le celle si possono modificare creare un comando per quando vengono modificate
                            QAbstractItemModel *model = new QStandardItemModel(miscela.size(),4);
                            model->setHeaderData(0,Qt::Horizontal,QString("Nome composto"));
                            model->setHeaderData(1,Qt::Horizontal,QString("Massa"));
                            model->setHeaderData(2,Qt::Horizontal,QString("Moli"));
                            model->setHeaderData(3,Qt::Horizontal,QString("Massa molare"));
                            QModelIndex index;
                            QVariant value;
                            for(unsigned int i=0;i<miscela.size();++i){
                                index = model->index(i,0);
                                value = QString::fromStdString(miscela[i].getNome());
                                model->setData(index,value);
                                index = model->index(i,1);
                                value = miscela[i].getMassa();
                                model->setData(index,value);
                                index = model->index(i,2);
                                value = miscela[i].getNMoli();
                                model->setData(index,value);
                                index = model->index(i,3);
                                value = miscela[i].getMassaMolare();
                                model->setData(index,value);
                            }
                            QTableView* view = new QTableView;
                            view->setModel(model);
                        }else{
                            QMessageBox::critical(view,QString("Errore"),QString("Non è stato possibile creare il grafico"));
                        }
                    }
                }
            }
        }else if("Confrontare le temperature dei composti di una miscela"){
            Miscela miscela("Grafico a punti");
            QString fileName = QFileDialog::getOpenFileName(view,QString("Confronto temperature - selezionare una Miscela"),QDir::homePath(),QString("*.json"));
            if(!fileName.isEmpty()){
                QFile file(fileName);
                if(!file.open(QIODevice::ReadOnly)){
                    QMessageBox::critical(view,QString("Errore nella lettura del file"),QString("C'è stato un problema di lettura del file."));
                    file.close();
                }else{
                    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
                    if(!doc.isObject() || (!doc.object().contains("tipologia")||doc.object()["tipologia"]!="miscela")){
                        QMessageBox::critical(view,QString("Errore nella lettura del file"),QString("C'è stato un problema di lettura del file."));
                    }else{
                        miscela = lettura_miscela(&file);
                        if(!miscela.empty()){
                            DatiGrafico grafico(QString::fromStdString(miscela.getNome()),
                                                QString::fromStdString(miscela.getNome()),
                                                DatiGrafico::linea,miscela.size(),20);
                            DatiPunto punto;
                            for(unsigned int i=0;i<miscela.size();++i){
                                punto.second = QString::fromStdString(miscela[i].getNome());
                                punto.first.setX((qreal)(i+1));
                                punto.first.setY(miscela[i].getTemperatura());
                                grafico.setPunto(punto,0,i);
                            }
                            //model.append(miscela);
                            view->aggiungiGrafico(&grafico);
                            //aggiungere una finestra a parte per visualizzare i dati di ogni composto(Datigrafico)
                            //controllare se la tabella e' visibile e se le celle sono modificabili
                            //se le celle si possono modificare creare un comando per quando vengono modificate
                            QAbstractItemModel *model = new QStandardItemModel(miscela.size(),2);
                            model->setHeaderData(0,Qt::Horizontal,QString("Nome composto"));
                            model->setHeaderData(1,Qt::Horizontal,QString("Temperatura"));
                            QModelIndex index;
                            QVariant value;
                            for(unsigned int i=0;i<miscela.size();++i){
                                index = model->index(i,0);
                                value = QString::fromStdString(miscela[i].getNome());
                                model->setData(index,value);
                                index = model->index(i,1);
                                value = miscela[i].getTemperatura();
                                model->setData(index,value);
                            }
                            QTableView* view = new QTableView;
                            view->setModel(model);
                        }else{
                            QMessageBox::critical(view,QString("Errore"),QString("Non è stato possibile creare il grafico"));
                        }
                    }
                }
            }
        }else if("Analizzare la molarità (rapporto moli e volume)"){
            Composto composto;
            Miscela miscela("Grafico a linea");
            QString fileName = QFileDialog::getOpenFileName(view,QString("Molarità - selezionare una Miscela"),QDir::homePath(),QString("*.json"));
            if(!fileName.isEmpty()){
                QFile file(fileName);
                if(!file.open(QIODevice::ReadOnly)){
                    QMessageBox::critical(view,QString("Errore nella lettura del file"),QString("C'è stato un problema di lettura del file."));
                    file.close();
                }else{
                    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
                    if(!doc.isObject() || (!doc.object().contains("tipologia")||doc.object()["tipologia"]!="miscela")){
                        QMessageBox::critical(view,QString("Errore nella lettura del file"),QString("C'è stato un problema di lettura del file."));
                    }else{
                        miscela = lettura_miscela(&file);
                        if(!miscela.empty() && miscela.isSoluzione()){
                            DatiGrafico grafico(QString::fromStdString(miscela.getNome()),
                                                QString::fromStdString(miscela.getNome()),
                                                DatiGrafico::linea,miscela.size(),20);
                            DatiPunto punto;
                            for(unsigned int i=0;i<miscela.size();++i){
                                punto.second = QString::fromStdString(miscela[i].getNome());
                                punto.first.setX((qreal)(i+1));
                                punto.first.setY(miscela.molarita(miscela[i]));
                                grafico.setPunto(punto,0,i);
                            }
                            //model.append(miscela);
                            view->aggiungiGrafico(&grafico);
                            //aggiungere una finestra a parte per visualizzare i dati di ogni composto(Datigrafico)
                            //controllare se la tabella e' visibile e se le celle sono modificabili
                            //se le celle si possono modificare creare un comando per quando vengono modificate
                            QAbstractItemModel *model = new QStandardItemModel(miscela.size(),3);
                            model->setHeaderData(0,Qt::Horizontal,QString("Nome composto"));
                            model->setHeaderData(1,Qt::Horizontal,QString("Moli Soluto"));
                            model->setHeaderData(2,Qt::Horizontal,QString("Molarità"));
                            QModelIndex index;
                            QVariant value;
                            for(unsigned int i=0;i<miscela.size();++i){
                                index = model->index(i,0);
                                value = QString::fromStdString(miscela[i].getNome());
                                model->setData(index,value);
                                index = model->index(i,1);
                                value = miscela.massaSoluto(miscela[i]);
                                model->setData(index,value);
                                index = model->index(i,2);
                                value = miscela.molarita(miscela[i]);
                                model->setData(index,value);
                            }
                            QTableView* view = new QTableView;
                            view->setModel(model);
                        }else{
                            QMessageBox::critical(view,QString("Errore"),QString("Non è stato possibile creare il grafico"));
                        }
                    }
                }
            }
        }
    }
}

void Controller::apri_file(){
    QString fileName = QFileDialog::getOpenFileName(view,QString("Apri file"),QDir::homePath(),QString("*.json"));
    if(fileName.isEmpty()) return;
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::critical(view,QString("Errore nella lettura del file"),QString("C'è stato un problema di lettura del file."));
        file.close();
        return;
    }
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if(!doc.isObject() || doc.object()["tipologia"]!=QString("composto")){
        QMessageBox::critical(view,QString("Errore nella lettura del file"),QString("C'è stato un problema di lettura del file."));
        file.close();
        return;
    }
    fileName = doc.object()["tipologia"].toString();
    if(fileName == QString("composto")){

    }else if(fileName == QString("miscela")){

    }

}

void Controller::salva_file(QVector<DatiGrafico*> window){
    for(int i=0;i<window.count();++i){
        if(window.at(i)->getTipo()==DatiGrafico::linea){

        }else if(window.at(i)->getTipo()==DatiGrafico::punti){

        }else if(window.at(i)->getTipo()==DatiGrafico::torta){

        }
    }
}

/*
void Controller::menu_file_apri(){
    /*
     * si aprira' un semplice QFileDialog dove sara' possibile indicare il file json con i dati del grafico interessato.
     * /
    QString fileName = QFileDialog::getOpenFileName(this,tr("Nuova Miscela - selezionare Composto"),QDir::homePath(),tr("*.json"));
    if(fileName.isEmpty()) return;
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::critical(this,tr("Errore nella lettura del file"),tr("C'è stato un problema di lettura del file."));
        file.close();
        return;
    }
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if(!doc.isObject() && !doc.object().contains("tipologia")){
        QMessageBox::critical(this,tr("Errore nella lettura del file"),tr("C'è stato un problema di lettura del file."));
        return;
    }else{
        if(doc.object()["tipologia"]==tr("composto")){
            Composto c = lettura_composto(&file);

            DatiGrafico g;
            aggiungiGrafico(&g);
        }else if(doc.object()["tipologia"]==tr("miscela")){
            Campione::Stato s = Campione::indefinito;
            if(doc.object()["stato"].toString()==tr("indefinito"))
                s = Campione::indefinito;
            else if(doc.object()["stato"].toString()==tr("solido"))
                s = Campione::solido;
            else if(doc.object()["stato"].toString()==tr("liquido"))
                s = Campione::liquido;
            else if(doc.object()["stato"].toString()==tr("gassoso"))
                s = Campione::gassoso;
            Miscela m(doc.object()["nome"].toString().toStdString(),s,
                    doc.object()["massa"].toDouble(),
                    doc.object()["volume"].toDouble(),
                    doc.object()["temperatura"].toDouble(),
                    doc.object()["soluzione"].toBool());
            QJsonArray lista = doc.object()["lista"].toArray();
            for(int i=0;i<lista.count();++i){
                QFile f(doc.object()["lista"].toArray().at(i).toString());
                if(!file.open(QIODevice::ReadOnly)){
                    QMessageBox::critical(this,tr("Errore nella lettura del file"),tr("C'è stato un problema di lettura del file."));
                    i = lista.count();
                }else{
                    Composto c = lettura_composto(&file);
                    if(!c.getNome().empty()){
                        m.push_back(c);
                    }
                }
            }

            DatiGrafico g;
            aggiungiGrafico(&g);
        }else{
            QMessageBox::critical(this,tr("Errore nella lettura del file"),tr("C'è stato un problema di lettura del file."));
        }
    }
    file.close();
}
*/

/*
void Controller::menu_file_salva(){
    /*
     * saranno controllate tutti i
     * /

}
*/
