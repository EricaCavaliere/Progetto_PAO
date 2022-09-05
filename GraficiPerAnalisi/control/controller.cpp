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
#include <QDebug>

Controller::Controller(QWidget* parent):QWidget(parent),view(new MainWindow(parent)){
    view->show();
    connect(view,&MainWindow::nuovo,this,&Controller::nuovo_file);
    connect(view,&MainWindow::apri,this,&Controller::apri_file);
    connect(view,&MainWindow::salva,this,&Controller::salva_file);
}

Controller::~Controller(){
    for(int i=0;i<model.count();++i)
        delete model[i].first;
}

bool Controller::isFileOpenRead(QFile& file){
    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::critical(view,QString("Errore di lettura"),QString("C'è stato un problema di lettura."));
        return false;
    }return true;
}

bool Controller::isFileOpenWrite(QFile& file){
    if(!file.open(QIODevice::ReadWrite)){
        QMessageBox::critical(view,QString("Errore di lettura"),QString("C'è stato un problema di lettura."));
        return false;
    }return true;
}

void Controller::insertInModel(Campione* c,QString s){
    Path path(c,s);
    model.push_back(path);
}

//rendere possibile salvare le modifiche alle celle
void Controller::viewTable(QAbstractItemModel* model){
    QTableView* table = new QTableView;
    table->setModel(model);
    table->show();
    connect(table->model(),&QAbstractItemModel::dataChanged,this,&Controller::modifica_tabella);
}

Campione::Stato Controller::fromStringToStato(QString stato){
    if(stato==QString("solido"))
        return Campione::solido;
    if(stato==QString("liquido"))
        return Campione::liquido;
    if(stato==QString("gassoso"))
        return Campione::gassoso;
    return Campione::indefinito;
}

QString Controller::fromStatoToString(Campione::Stato stato){
    if(stato==Campione::solido)
        return QString("solido");
    if(stato==Campione::liquido)
        return QString("liquido");
    if(stato==Campione::gassoso)
        return QString("gassoso");
    return "indefinito";
}

DatiGrafico::TipoGrafico Controller::fromStringToGrafico(QString tipo){
    if(tipo=="torta"){
        return DatiGrafico::torta;
    }else if(tipo=="punti"){
        return DatiGrafico::punti;
    }else{
        return DatiGrafico::linea;
    }
}

QString Controller::fromGraficoToString(DatiGrafico::TipoGrafico tipo){
    if(tipo==DatiGrafico::torta){
        return "torta";
    }else if(tipo==DatiGrafico::punti){
        return "punti";
    }else{
        return "linea";
    }
}

Elemento Controller::crea_elemento(){
    bool ok;
    QString nome = QInputDialog::getText(view,QString("Nuovo Elemeneto"),QString("Inserire il nome o la formula dell'elemento:"),QLineEdit::Normal,QString(),&ok);
    if(ok && !nome.isEmpty()){
        int nAtomico = QInputDialog::getInt(view,QString("Nuovo Elemento"),QString("Inserire il numero atomico:"),0,0,200,1,&ok);
        if(ok){
            double nMassa = QInputDialog::getDouble(view,QString("Nuovo Elemento"),QString("Inserire il numero di massa:"),0,0,500,4,&ok);
            if(ok){
                QFile file(QDir::homePath()+"/elementi.json");
                if(!isFileOpenWrite(file)){
                    file.close();
                    return Elemento();
                }
                QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
                QJsonArray chiavi;
                if(doc.object().contains("chiavi"))
                    for(int i=0;i<doc.object()["chiavi"].toArray().count();++i){
                        chiavi.append(doc.object()["chiavi"].toArray().at(i));
                    }
                chiavi.append(QString::number(nAtomico));
                QJsonObject element;
                element.insert("nome",nome);
                element.insert("numAtomico",nAtomico);
                element.insert("numMassa",nMassa);
                QJsonObject obj;
                obj.insert("chiavi",chiavi);
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
                    for(int i=0;i<doc.object()["chiavi"].toArray().count();++i){
                        obj.insert(chiavi.at(i).toString(),doc.object()[chiavi.at(i).toString()].toObject());
                    }
                    file.resize(0);
                }
                obj.insert(QString::number(nAtomico),element);
                doc.setObject(obj);
                file.write(doc.toJson());
                file.close();
                QMessageBox::information(view,QString("Elemento creato"),QString("L'Elemento è stato creato con successo."));
                return Elemento(nome.toStdString(),nAtomico,nMassa);
            }
        }
    }
    return Elemento();
}

Composto Controller::crea_composto(){
    bool ok;
    QString formula = QInputDialog::getText(view,QString("Nuovo Composto"),QString("Indicare la formula chimica del composto:"),QLineEdit::Normal,QString(),&ok);
    if(!ok || formula.isEmpty()) return Composto();
    QString nome = QInputDialog::getText(view,QString("Nuovo Composto"),QString("Indicare il nome del composto analizzato:"),QLineEdit::Normal,QString(),&ok);
    if(!ok || nome.isEmpty()) return Composto();
    QStringList items;
    items << QString("indefinito") << QString("solido") << QString("liquido") << QString("gassoso");
    QString stato = QInputDialog::getItem(view,QString("Nuovo Composto"),QString("Indicare lo stato del composto:"),items,0,false,&ok);
    double massa(0.0),volume(0.0),temperatura(0.0);
    int moli(0);
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
    Composto composto(formula.toStdString(),nome.toStdString(),fromStringToStato(stato),massa,volume,temperatura,moli);
    QFile file(QDir::homePath()+"/elementi.json");
    if(isFileOpenRead(file)){
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        if(!doc.isObject()){
            QMessageBox::StandardButton button;
            button = QMessageBox::question(view,QString("Nuovo Composto"),QString("Non ci sono elementi da poter inserire, vuole creare un elemento?"), QMessageBox::Yes | QMessageBox::No);
            while(button==QMessageBox::Yes){
                Elemento e(crea_elemento());
                if(e.getNome().empty()){
                    QMessageBox::critical(view,QString("Composto non creato"),QString("Ci sono stati degli errori nella creazione del compposto."));
                    file.close();
                    return Composto();
                }
                composto.push_back(e);
                button = QMessageBox::question(view,QString("Nuovo Composto"),QString("Vuole creare un altro elemento?"), QMessageBox::Yes | QMessageBox::No);
            }
        }else{
            items.clear();
            ok = true;
            for(int i=0;i<doc.object()["chiavi"].toArray().count();++i){
                items << doc.object()[doc.object()["chiavi"].toArray().at(i).toString()].toObject()["nome"].toString()
                        +QString("-")+doc.object()["chiavi"].toArray().at(i).toString();
            }
            while(!items.empty() && ok){
                QString n = QInputDialog::getItem(view,QString("Nuovo Composto"),QString("Inserire un elemento.\nCliccare OK per inserire un Elemento nel Composto, altrimenti cliccare Annulla per andare al grafico."),items,0,false,&ok);
                if(ok){
                    items.removeOne(n);
                    QStringList s;
                    s = n.split("-");
                    Elemento e(doc.object()[s.back()].toObject()["nome"].toString().toStdString(),
                            doc.object()[s.back()].toObject()["numAtomico"].toInt(),
                            doc.object()[s.back()].toObject()["numMassa"].toDouble());
                    composto.push_back(e);
                }
            }
        }
    }
    file.close();
    return composto;
}

Miscela Controller::crea_miscela(){
    bool ok;
    QString nome = QInputDialog::getText(view,QString("Nuova Miscela"),QString("Indicare il nome dela miscela:"),QLineEdit::Normal,QString(),&ok);
    if(!ok || nome.isEmpty()) return Miscela();
    QStringList items;
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
                if(ok){
                    QMessageBox::StandardButton button;
                    button = QMessageBox::question(view,QString("Nuova Miscela"),QString("La Miscela analizzata è una soluzione?"), QMessageBox::Yes | QMessageBox::No);
                    if(button==QMessageBox::No)soluzione=false;
                }
            }
        }
    }
    Miscela miscela(nome.toStdString(),fromStringToStato(stato),massa,volume,temperatura,soluzione);
    ok=true;
    while(ok){
        QString fileName = QFileDialog::getOpenFileName(view,QString("Nuova Miscela - selezionare un Composto"),QDir::homePath(),QString("*.json"));
        if(!fileName.isEmpty()){
            QFile file(fileName);
            if(isFileOpenRead(file)){
                QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
                if(doc.isObject() && doc.object().contains("tipologia") && doc.object()["tipologia"]=="composto"){
                    Composto composto(lettura_composto(doc));
                    if(!composto.getNome().empty())
                        miscela.push_back(composto);
                }else
                    QMessageBox::critical(view,QString("Errore"),QString("Non è stato possibile aprire il file indicato."));
            }
            file.close();
        }else ok=false;
    }
    return miscela;
}

Composto Controller::lettura_composto(QJsonDocument& doc){
    Composto composto(doc.object()["formula"].toString().toStdString(),
            doc.object()["nome"].toString().toStdString(),
            fromStringToStato(doc.object()["stato"].toString()),
            doc.object()["massa"].toDouble(),
            doc.object()["volume"].toDouble(),
            doc.object()["temperatura"].toDouble(),
            doc.object()["moli"].toInt());
    QFile elementi(QDir::homePath()+"/elementi.json");
    if(!isFileOpenRead(elementi)){
        elementi.close();
        return Composto();
    }
    QJsonDocument docElementi = QJsonDocument::fromJson(elementi.readAll());
    if(!docElementi.isObject()){
        QMessageBox::critical(view,QString("Errore"),QString("Non è stato possibile aprire il Composto indicato."));
        return Composto();
    }else{
        QJsonArray lista = doc.object()["lista"].toArray();
        for(int i=0;i<lista.count();++i){
            Elemento e(docElementi.object()[doc.object()["lista"].toArray().at(i).toString()].toObject()["nome"].toString().toStdString(),
                    docElementi.object()[doc.object()["lista"].toArray().at(i).toString()].toObject()["numAtomico"].toInt(),
                    docElementi.object()[doc.object()["lista"].toArray().at(i).toString()].toObject()["numMassa"].toDouble());
            composto.push_back(e);
        }
    }
    elementi.close();
    return composto;
}

Miscela Controller::lettura_miscela(QJsonDocument &doc){
    Miscela miscela(doc.object()["nome"].toString().toStdString(),
            fromStringToStato(doc.object()["stato"].toString()),
            doc.object()["massa"].toDouble(),
            doc.object()["volume"].toDouble(),
            doc.object()["temperatura"].toDouble(),
            doc.object()["soluzione"].toBool());
    for(auto i = doc.object()["lista"].toArray().begin();i!=doc.object()["lista"].toArray().end();++i){
        Composto c(i->toObject()["formula"].toString().toStdString(),
                i->toObject()["nome"].toString().toStdString(),
                fromStringToStato(i->toObject()["stato"].toString()),
                i->toObject()["massa"].toDouble(),
                i->toObject()["volume"].toDouble(),
                i->toObject()["temperatura"].toDouble(),
                i->toObject()["moli"].toInt());
        QFile elementi(QDir::homePath()+"/elementi.json");
        if(!isFileOpenRead(elementi)){
            elementi.close();
            return Miscela();
        }
        QJsonDocument docElementi = QJsonDocument::fromJson(elementi.readAll());
        if(!docElementi.isObject()){
            QMessageBox::critical(view,QString("Errore"),QString("Non è stato possibile leggere il Componente indicato."));
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
        miscela.push_back(c);
    }
    return miscela;
}

DatiGrafico Controller::lettura_grafico(QJsonDocument &doc){
    unsigned int nLinee = doc.object()["grafico"].toObject().count()-3;
    unsigned int nPunti;
    if(nLinee>0)
        nPunti = doc.object()["grafico"].toObject()[QString::number(0)].toArray().count();
    else nPunti=0;
    DatiGrafico g(doc.object()["grafico"].toObject()["titolo"].toString(),
            doc.object()["grafico"].toObject()["intestazione"].toString(),
            fromStringToGrafico(doc.object()["grafico"].toObject()["tipologia"].toString()),0,0,0);
    if(nPunti==0) return g;
    for(unsigned int i=0;i<nLinee;++i){
        DatiLinea l;
        for(auto j=doc.object()["grafico"].toObject()[QString::number(i)].toArray().begin();j!=doc.object()["grafico"].toObject()[QString::number(i)].toArray().end();++j){
            DatiPunto p;
            p.first.setX((qreal) j->toObject()["x"].toDouble());
            p.first.setY((qreal) j->toObject()["y"].toDouble());
            p.second = j->toObject()["nome"].toString();
            l.push_back(p);
        }
        g.tabella.push_back(l);
    }
    return g;
}

void Controller::salva_composto_json(QJsonObject& obj,Composto* composto, const DatiGrafico& d){
    obj.insert("formula",QString::fromStdString(composto->getFormulaChimica()));
    obj.insert("nome",QString::fromStdString(composto->getNome()));
    obj.insert("stato",fromStatoToString(composto->getMateria()));
    obj.insert("massa",composto->getMassa());
    obj.insert("volume",composto->getVolume());
    obj.insert("temperatura",composto->getTemperatura());
    obj.insert("moli",QString(composto->getNMoli()).toInt());
    QJsonArray lista;
    for(auto j=composto->begin();j!=composto->end();++j)
        lista.push_back(QString::number(j->getNumAtomico()));
    obj.insert("lista",lista);
    QJsonObject grafico;
    imposta_tabella_json(grafico,d);
    obj.insert("grafico",grafico);
}

void Controller::salva_miscela_json(QJsonObject& obj,Miscela* miscela,const DatiGrafico& d){
    obj.insert("nome",QString::fromStdString(miscela->getNome()));
    obj.insert("stato",fromStatoToString(miscela->getMateria()));
    obj.insert("massa",miscela->getMassa());
    obj.insert("volume",miscela->getVolume());
    obj.insert("temperatura",miscela->getTemperatura());
    obj.insert("soluzione",miscela->isSoluzione());
    QJsonArray lista;
    for(auto i = miscela->begin();i!=miscela->end();++i){
        QJsonObject c;
        c.insert("formula",QString::fromStdString(i->getFormulaChimica()));
        c.insert("nome",QString::fromStdString(i->getNome()));
        c.insert("stato",fromStatoToString(i->getMateria()));
        c.insert("massa",i->getMassa());
        c.insert("volume",i->getVolume());
        c.insert("temperatura",i->getTemperatura());
        c.insert("moli",QString(i->getNMoli()).toInt());
        QJsonArray array;
        for(auto j=i->begin();j!=i->end();++j){
            qDebug() << j->getNumAtomico();
            array.push_back(QString::number(j->getNumAtomico()));
        }
        c.insert("lista",array);
        lista.push_back(c);
    }
    obj.insert("lista",lista);
    QJsonObject grafico;
    imposta_tabella_json(grafico,d);
    obj.insert("grafico",grafico);
}

void Controller::imposta_tabella_json(QJsonObject& grafico, const DatiGrafico& d){
    grafico.insert("titolo",d.getTitolo());
    grafico.insert("intestazione",d.getIntestazione());
    grafico.insert("tipologia",fromGraficoToString(d.getTipo()));
    unsigned int i=0;
    for(auto j=d.tabella.begin();j!=d.tabella.end();++j){
        QJsonArray linea;
        for(auto z = j->begin();z!=j->end();++z){
            QJsonObject punto;
            punto.insert("x",z->first.x());
            punto.insert("y",z->first.y());
            punto.insert("nome",z->second);
            linea.push_back(punto);
        }
        grafico.insert(QString::number(i),linea);
        i++;
    }
}


//da completare
void Controller::nuovo_file(){
    bool ok;
    QStringList items;
    items << QString("Elemento") << QString("Composto") << QString("Miscela") << QString("Altro");
    QString s = QInputDialog::getItem(view,QString("Nuovo grafico"),QString("Che cosa vuole creare?"),items,0,false,&ok);
    if(!ok) return;
    if(s==QString("Elemento")){
        crea_elemento();
    }else if(s==QString("Composto")){
        Composto composto(crea_composto());
        if(composto.getNome()=="")return;
        DatiGrafico grafico(QString::fromStdString(composto.getFormulaChimica()),QString("Composto"),DatiGrafico::torta,composto.size());
        for(unsigned int i=0; i<composto.size();++i){
            grafico.tabella[0][i].second = QString::fromStdString(composto[i].getNome());
            grafico.tabella[0][i].first.setX(0.0);
            grafico.tabella[0][i].first.setY(1.0);
        }
        insertInModel(new Composto(composto),QString());
        view->aggiungiGrafico(grafico);
    }else if(s==QString("Miscela")){
        Miscela miscela(crea_miscela());
        if(miscela.getNome()=="") return;
        unsigned int nmax = 0;
        for(Miscela::iterator i=miscela.begin();i!=miscela.end();++i){
            if(i->size()>nmax) nmax = i->size();
        }
        DatiGrafico grafico(QString::fromStdString(miscela.getNome()),QString("Miscela"),DatiGrafico::torta,nmax,20,miscela.size());
        for(unsigned int i=0;i<miscela.size();++i){
            for(unsigned int j=0;j<miscela[i].size();++j){
                grafico.tabella[i][j].second = QString::fromStdString(miscela[i][j].getNome());
                grafico.tabella[i][j].first.setX(0.0);
                grafico.tabella[i][j].first.setY(1.0);
            }
            for(unsigned int j=miscela[i].size();j<nmax;++j){
                grafico.tabella[i][j].second = QString("");
                grafico.tabella[i][j].first.setX(0.0);
                grafico.tabella[i][j].first.setY(0.0);
            }
        }
        insertInModel(new Miscela(miscela),QString());
        view->aggiungiGrafico(grafico);
    }else{
        items.clear();
        items << QString("Analizzare la massa molare dei composti di una miscela") //linea
              << QString("Confrontare le temperature dei composti di una miscela") //punti
              << QString("Analizzare la molarità di una miscela (rapporto moli e volume)");//linea
        s = QInputDialog::getItem(view,QString("Nuovo grafico"),QString("Che cosa vuole creare?"),items,0,false,&ok);
        if(!ok) return;
        if(s=="Analizzare la massa molare dei composti di una miscela"){
            Miscela miscela("Massa molare");
            QString fileName = QFileDialog::getOpenFileName(view,QString("Massa molare - selezionare un Composto o una Miscela"),QDir::homePath(),QString("*.json"));
            if(fileName.isEmpty()) return;
            QFile file(fileName);
            if(!isFileOpenRead(file)){
                file.close();
                return;
            }
            QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
            if(!doc.isObject() || !doc.object().contains("tipologia") || doc.object()["tipologia"]!="miscela"){
                QMessageBox::critical(view,QString("Errore nella lettura del file"),QString("C'è stato un problema di lettura del file."));
                file.close();
                return;
            }
            miscela = lettura_miscela(doc);
            file.close();
            if(miscela.empty()){
                QMessageBox::critical(view,QString("Errore"),QString("Non è stato possibile creare il grafico"));
                return;
            }
            DatiGrafico grafico(QString::fromStdString(miscela.getNome()),QString("Massa molare"),DatiGrafico::linea,miscela.size());
            for(unsigned int i=0;i<miscela.size();++i){
                grafico.tabella[0][i].second = QString::fromStdString(miscela[i].getNome());
                grafico.tabella[0][i].first.setX(i+1);
                grafico.tabella[0][i].first.setY((qreal)miscela[i].getMassaMolare());
            }
            insertInModel(new Miscela(miscela),QString());
            view->aggiungiGrafico(grafico);
            /*
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
            viewTable(model);
            */
        }else if(s=="Confrontare le temperature dei composti di una miscela"){
            Miscela miscela("Temperatura");
            QString fileName = QFileDialog::getOpenFileName(view,QString("Confronto temperature - selezionare una Miscela"),QDir::homePath(),QString("*.json"));
            if(fileName.isEmpty()) return;
            QFile file(fileName);
            if(isFileOpenRead(file)){
                file.close();
                return;
            }
            QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
            if(!doc.isObject() || (!doc.object().contains("tipologia")||doc.object()["tipologia"]!="miscela")){
                QMessageBox::critical(view,QString("Errore nella lettura del file"),QString("C'è stato un problema di lettura del file."));
                file.close();
                return;
            }
            miscela = lettura_miscela(doc);
            if(miscela.empty()){
                QMessageBox::critical(view,QString("Errore"),QString("Non è stato possibile creare il grafico"));
                return;
            }
            DatiGrafico grafico(QString::fromStdString(miscela.getNome()),QString("Temperatura"),DatiGrafico::linea,miscela.size(),20);
            for(unsigned int i=0;i<miscela.size();++i){
                grafico.tabella[0][i].second = QString::fromStdString(miscela[i].getNome());
                grafico.tabella[0][i].first.setX((qreal)(i+1));
                grafico.tabella[0][i].first.setY((qreal) miscela[i].getTemperatura());
            }
            insertInModel(new Miscela(miscela),QString());
            view->aggiungiGrafico(grafico);
            /*
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
            viewTable(model);
            */
        }else if(s=="Analizzare la molarità di una miscela (rapporto moli e volume)"){
            Composto composto;
            Miscela miscela("Molarità");
            QString fileName = QFileDialog::getOpenFileName(view,QString("Molarità - selezionare una Miscela"),QDir::homePath(),QString("*.json"));
            if(fileName.isEmpty())return;
            QFile file(fileName);
            if(!isFileOpenRead(file)){
                file.close();
                return;
            }
            QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
            if(!doc.isObject() || (!doc.object().contains("tipologia")||doc.object()["tipologia"]!="miscela")){
                QMessageBox::critical(view,QString("Errore nella lettura del file"),QString("C'è stato un problema di lettura del file."));
                file.close();
                return;
            }
            miscela = lettura_miscela(doc);
            if(miscela.empty() || !miscela.isSoluzione()){
                QMessageBox::critical(view,QString("Errore"),QString("Non è stato possibile creare il grafico"));
                file.close();
                return;
            }
            DatiGrafico grafico(QString::fromStdString(miscela.getNome()),QString("Molarità"),DatiGrafico::linea,miscela.size(),20);
            DatiPunto punto;
            for(unsigned int i=0;i<miscela.size();++i){
                grafico.tabella[0][i].second = QString::fromStdString(miscela[i].getNome());
                grafico.tabella[0][i].first.setX((qreal)(i+1));
                grafico.tabella[0][i].first.setY((qreal)miscela.molarita(miscela[i]));
            }
            insertInModel(new Miscela(miscela),QString());
            view->aggiungiGrafico(grafico);
            /*
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
            viewTable(model);
            */
        }
    }
}

//da completare
void Controller::apri_file(){
    QString fileName = QFileDialog::getOpenFileName(view,QString("Apri file"),QDir::homePath(),QString("*.json"));
    if(fileName.isEmpty()) return;
    for(int i=0;i<model.count();++i){
        if(fileName==model.at(i).second){
            QMessageBox::information(view,QString("File aperto"),QString("Il file indicato è già aperto."));
            return;
        }
    }
    QFile file(fileName);
    if(!isFileOpenRead(file)){
        file.close();
        return;
    }
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if(!doc.isObject()){
        QMessageBox::critical(view,QString("Errore nella lettura del file"),QString("C'è stato un problema di lettura del file."));
        file.close();
        return;
    }
    QString tipologia = doc.object()["tipologia"].toString();
    if(tipologia == QString("composto")){
        Composto c(lettura_composto(doc));
        if(c.getNome()=="")return;
        insertInModel(new Composto(c),fileName);
        view->aggiungiGrafico(lettura_grafico(doc));
    }else if(tipologia == QString("miscela")){
        Miscela m(lettura_miscela(doc));
        if(m.getNome()=="")return;
        insertInModel(new Miscela(m),fileName);
        view->aggiungiGrafico(lettura_grafico(doc));
    }else if(tipologia == QString("massa molare")){

    }else if(tipologia == QString("temperatura")){

    }else if(tipologia == QString("molarita")){

    }
    file.close();
}

//da completare
void Controller::salva_file(const QVector<DatiGrafico> &window){
    for(int i=0;i<window.count();++i){
        QMessageBox::StandardButton ok = QMessageBox::question(view,QString("Salva file"),QString("Vuole salvare il grafico ")+QString::fromStdString(model.at(i).first->getNome())+QString("?"),QMessageBox::Yes | QMessageBox::No);
        //identificare il file o crearne uno
        QString fileName = model.at(i).second;
        if(ok==QMessageBox::Yes && (fileName.isEmpty()))
            fileName = QFileDialog::getSaveFileName(view,QString("Salva il grafico ")+QString::fromStdString(model.at(i).first->getNome()),QDir::homePath(),QString("*.json"));
        while(fileName==QDir::homePath()+tr("/elementi.json")){
            QMessageBox::warning(view,tr("File non modificabile"),tr("Non è posibile salvare il grafico nel file elementi.json."));
            fileName = QFileDialog::getSaveFileName(view,QString("Salva il grafico ")+QString::fromStdString(model.at(i).first->getNome()),QDir::homePath(),QString("*.json"));
        }
        if(ok==QMessageBox::Yes && !fileName.isEmpty()){
            model[i].second = fileName;
            QFile file(fileName);
            if(!isFileOpenWrite(file)){
                file.close();
                return;
            }
            QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
            QJsonObject obj;
            //salvare i dati
            if(window.at(i).getTipo()==DatiGrafico::linea){

            }else if(window.at(i).getTipo()==DatiGrafico::punti){
                /*Miscela* miscela = dynamic_cast<Miscela*>(&model[i].first);
                obj.insert("tipologia","temperatura");
                salva_miscela_json(obj,miscela);*/
            }else if(window.at(i).getTipo()==DatiGrafico::torta){
                Composto* composto = dynamic_cast<Composto*>(model[i].first);
                Miscela* miscela = dynamic_cast<Miscela*>(model[i].first);
                if(composto){
                    obj.insert("tipologia","composto");
                    salva_composto_json(obj,composto,window.at(i));
                }else if(miscela){
                    obj.insert("tipologia","miscela");
                    salva_miscela_json(obj,miscela,window.at(i));
                }
            }
            //indicare se il salvataggio e' stato eseguito con successo
            doc.setObject(obj);
            file.resize(0);
            file.write(doc.toJson());
            file.close();
            QMessageBox::information(view,QString("File salvato"),QString("Il grafico è stato salvato con successo nel seguente percorso:\n")+fileName);
        }
    }
}

//da pensare a come implementarla e se implementarla
void Controller::modifica_tabella(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles){}

