#include "controller.h"
#include "model/campione.h"
#include "model/miscela.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTextStream>
#include <QDir>
#include <QDebug>
#include <QAction>
#include <QFileDialog>


Controller::Controller():MainWindow(){
    DatiGrafico g;
    aggiungiGrafico(&g);
    show();
}

Elemento Controller::inserire_elemento(){
    bool ok;
    QString nome = QInputDialog::getText(this,tr("Nuovo Elemeneto"),tr("Inserire il nome o la formula dell'elemento:"),QLineEdit::Normal,QString(),&ok);
    if(ok && !nome.isEmpty()){
        int nAtomico = QInputDialog::getInt(this,tr("Nuovo Elemento"),tr("Inserire il numero atomico:"),0,0,200,1,&ok);
        if(ok){
            double nMassa = QInputDialog::getDouble(this,tr("Nuovo Elemento"),tr("Inserire il numero di massa:"),0,0,500,4,&ok);
            if(ok){
                QFile file(QDir::homePath()+"/elementi.json");
                if(!file.open(QIODevice::ReadWrite)){
                    QMessageBox::critical(this,tr("Errore nella lettura del file"),tr("C'è stato un problema di lettura del file \"elementi.json\" per poter inserire il nuovo elemento nella lista."));
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
                        QMessageBox::warning(this,tr("Elemento esistente"),
                                             tr("Esiste un elemento con lo stesso numero atomico\nNome: ")+
                                             doc.object()[QString::number(nAtomico)].toObject()["nome"].toString()+
                                             tr("\nNumero atomico: ")+
                                             QString::number(doc.object()[QString::number(nAtomico)].toObject()["numAtomico"].toInt())+
                                             tr("\nNumero di massa: ")+
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

Composto Controller::lettura_composto(QFile* file){
    QJsonDocument doc = QJsonDocument::fromJson(file->readAll());
    if(!doc.isObject() || doc.object()["tipologia"]!=tr("composto")){
        QMessageBox::critical(this,tr("Errore nella lettura del file"),tr("C'è stato un problema di lettura del file."));
        return Composto();
    }
    Campione::Stato s = Campione::indefinito;
    if(doc.object()["stato"].toString()==tr("indefinito"))
        s = Campione::indefinito;
    else if(doc.object()["stato"].toString()==tr("solido"))
        s = Campione::solido;
    else if(doc.object()["stato"].toString()==tr("liquido"))
        s = Campione::liquido;
    else if(doc.object()["stato"].toString()==tr("gassoso"))
        s = Campione::gassoso;
    Composto c(doc.object()["formula"].toString().toStdString(),
            doc.object()["nome"].toString().toStdString(),s,
            doc.object()["massa"].toDouble(),
            doc.object()["volume"].toDouble(),
            doc.object()["temperatura"].toDouble(),
            doc.object()["moli"].toInt());
    QFile elementi(QDir::homePath()+"/elementi.json");
    if(!elementi.open(QIODevice::ReadOnly)){
        QMessageBox::critical(this,tr("Errore nella lettura del file"),tr("C'è stato un problema di lettura del file \"elementi.json\" per poter leggere gli elementi nella lista."));
    }
    QJsonDocument docElementi = QJsonDocument::fromJson(elementi.readAll());
    if(!docElementi.isObject()){
        QMessageBox::critical(this,tr("Errore grave"),tr("Non è stato possibile aprire il Componente indicato."));
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


void Controller::menu_file_nuovo(){
    bool ok;
    QStringList items;
    items << tr("Elemento") << tr("Composto") << tr("Miscela");
    QString s = QInputDialog::getItem(this,tr("Nuovo grafico"),tr("Che cosa vuole creare?"),items,0,false,&ok);
    if(!ok) return;

    if(s==tr("Elemento")){
        inserire_elemento();
    }else if(s==tr("Composto")){
        bool ok;
        QString formula = QInputDialog::getText(this,tr("Nuovo Composto"),tr("Indicare la formula chimica del composto:"),QLineEdit::Normal,QString(),&ok);
        if(!ok || formula.isEmpty()) return;
        QString nome = QInputDialog::getText(this,tr("Nuovo Composto"),tr("Indicare il nome del composto analizzato:"),QLineEdit::Normal,QString(),&ok);
        if(!ok || nome.isEmpty()) return;
        items.clear();
        items << tr("indefinito") << tr("solido") << tr("liquido") << tr("gassoso");
        QString stato = QInputDialog::getItem(this,tr("Nuovo Composto"),tr("Indicare lo stato del composto:"),items,0,false,&ok);
        double massa=0.0,volume=0.0,temperatura=0.0;
        int moli=0;
        if(ok){
            massa = QInputDialog::getDouble(this,tr("Nuovo Composto"),tr("Inserire la massa:"),0,0,1000000,4,&ok);
            if(ok){
                volume = QInputDialog::getDouble(this,tr("Nuovo Composto"),tr("Inserire il volume:"),0,0,1000000,4,&ok);
                if(ok){
                    temperatura = QInputDialog::getDouble(this,tr("Nuovo Composto"),tr("Inserire la temperatura:"),0,-1000,1000,2,&ok);
                    if(ok)
                        moli = QInputDialog::getInt(this,tr("Nuovo Composto"),tr("Inserire la quantità di moli interessata:"),0,0,1000000);
                }
            }
        }
        QFile file(QDir::homePath()+"/elementi.json");
        if(!file.open(QIODevice::ReadOnly)){
            QMessageBox::critical(this,tr("Errore nella lettura del file"),tr("C'è stato un problema di lettura del file \"elementi.json\" per poter leggere gli elementi nella lista."));
        }
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        Campione::Stato s = Campione::indefinito;
        if(stato==tr("solido")) s = Campione::solido;
        else if(stato==tr("liquido")) s = Campione::liquido;
        else if(stato==tr("gassoso")) s = Campione::gassoso;
        Composto composto(formula.toStdString(),nome.toStdString(),s,massa,volume,temperatura,moli);
        if(!doc.isObject()){
            ok = QMessageBox::question(this,tr("Nuovo Composto"),tr("Non ci sono elementi da poter inserire, vuole creare un elemento?"));
            while(ok){
                Elemento e = inserire_elemento();
                if(e.getNome().empty()){
                    QMessageBox::critical(this,tr("Composto non creato"),tr("Ci sono stati degli errori nella creazione del compposto."));
                    file.close();
                    return;
                }
                composto.push_back(e);
                ok = QMessageBox::question(this,tr("Nuovo Composto"),tr("Vuole creare un altro elemento?"));
            }
        }else{
            items.clear();
            for(auto i=doc.object().begin();i!=doc.object().end();++i){
                items << doc.object()[i.key()].toObject()["nome"].toString()+tr("-")+i.key();
            }
            QString n = QInputDialog::getItem(this,tr("Nuovo Composto"),tr("Inserire un elemento.\nCliccare OK per inserire un altro Elemento nel Composto, altrimenti cliccare Annulla per andare al grafico."),items,0,false,&ok);
            QStringList s;
            while(ok){
                s = n.split("-");
                composto.push_back(Elemento(doc.object()[s.at(1)].toObject()["nome"].toString().toStdString(),
                        doc.object()[s.at(1)].toObject()["numAtomico"].toInt(),
                        doc.object()[s.at(1)].toObject()["numMassa"].toDouble()));
                n = QInputDialog::getItem(this,tr("Nuovo Composto"),tr("Inserire un elemento.\nCliccare OK per inserire un altro Elemento nel Composto, altrimenti cliccare Cancella per andare al grafico."),items,0,false,&ok);
            }
        }
        file.close();
        /*
        DatiGrafico g(nome,DatiGrafico::torta);
        DatiLinea l;
        DatiPunto p;
        ok = composto.begin()!=composto.end();
        qDebug() << ok;
        for(auto i=composto.begin();i!=composto.end();++i){
            p.first = QPointF((qreal)0,(qreal)1);
            p.second = QString::fromStdString(i->getNome());
            l.push_back(p);
            qDebug() << p.second;
        }
        g.addLinea(l);
        */
        DatiGrafico g;
        aggiungiGrafico(&g);
    }else if(s==tr("Miscela")){
        /*
         * Una miscela e' un insieme di composti.
         * Verra' aperto un QDialog dove sara' richiesto di indicare i composti di cui e' formato la miscela.
         * Ad ogni composto o viene creato a mano, o viene data la possibilita' di aprire un QFileDialog dove sara' possibile trovare il file json del composto interessato
         * Dopo aver inserito i Composti e i dati relativi alla Miscela, si dara' la possibilita' di vedere il grafico a video tramite il pulsante OK o annullare tutte le impostazioni tramite il pulsante ANNULLA o X.
         */
        bool ok;
        QString nome = QInputDialog::getText(this,tr("Nuova Miscela"),tr("Indicare il nome dela miscela:"),QLineEdit::Normal,QString(),&ok);
        if(!ok || nome.isEmpty()) return;
        items.clear();
        items << tr("indefinito") << tr("solido") << tr("liquido") << tr("gassoso");
        QString stato = QInputDialog::getItem(this,tr("Nuova Miscela"),tr("Indicare lo stato della miscela:"),items,0,false,&ok);
        double massa=0.0,volume=0.0,temperatura=0.0;
        bool soluzione=false;
        if(ok){
            massa = QInputDialog::getDouble(this,tr("Nuova Miscela"),tr("Inserire la massa:"),0,0,1000000,4,&ok);
            if(ok){
                volume = QInputDialog::getDouble(this,tr("Nuova Miscela"),tr("Inserire il volume:"),0,0,1000000,4,&ok);
                if(ok){
                    temperatura = QInputDialog::getDouble(this,tr("Nuova Miscela"),tr("Inserire la temperatura:"),0,-1000,1000,2,&ok);
                    if(ok)
                        soluzione = QMessageBox::question(this,tr("Nuova Miscela"),tr("La Miscela analizzata è una soluzione?"));
                }
            }
        }


        Campione::Stato s = Campione::indefinito;
        if(stato==tr("solido")) s = Campione::solido;
        else if(stato==tr("liquido")) s = Campione::liquido;
        else if(stato==tr("gassoso")) s = Campione::gassoso;
        Miscela miscela(nome.toStdString(),s,massa,volume,temperatura,soluzione);

        ok=true;
        while(ok){
            QString fileName = QFileDialog::getOpenFileName(this,tr("Nuova Miscela - selezionare Composto"),QDir::homePath(),tr("*.json"));
            if(!fileName.isEmpty()){
                QFile file(fileName);
                if(!file.open(QIODevice::ReadOnly)){
                    QMessageBox::critical(this,tr("Errore nella lettura del file"),tr("C'è stato un problema di lettura del file."));
                }
                Composto c = lettura_composto(&file);
                if(!c.getNome().empty()){
                    miscela.push_back(c);
                    file.close();
                }
            }else ok=false;
        }
        DatiGrafico g(nome,DatiGrafico::torta);
        DatiLinea l;
        DatiPunto p;
        ok = miscela.begin()!=miscela.end();
        qDebug() << ok;
        for(auto i=miscela.begin();i!=miscela.end();++i){
            p.first = QPointF((qreal)0,(qreal)1);
            p.second = QString::fromStdString(i->getNome());
            l.push_back(p);
            qDebug() << p.second;
        }
        g.addLinea(l);
        aggiungiGrafico(&g);
    }
}

void Controller::menu_file_apri(){
    /*
     * si aprira' un semplice QFileDialog dove sara' possibile indicare il file json con i dati del grafico interessato.
     */
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

void Controller::menu_file_salva(){
    /*
     * saranno controllate tutti i
     */
    for(int i=0;i<window.count();++i){
        QStringList list = window.at(i)->getTitolo().split('-');
        if(list.at(0)=="Composto"){

        }else{
            //vuol dire che e' una miscela
        }
    }
}
