#include "datigrafico.h"

#include <QTime>

DatiGrafico::DatiGrafico(QString s,QString i,TipoGrafico t,int nPunti, int valoreMax, int nLinee)
    :tabella(generateRandomData(nLinee,valoreMax,nPunti)),titolo(s),intestazione(i),tipo(t){}

DatiGrafico::DatiGrafico(const DatiGrafico &d)
    :tabella(d.tabella),titolo(d.titolo),intestazione(d.intestazione),tipo(d.tipo){}

DatiGrafico::~DatiGrafico(){}

/*
int DatiGrafico::getX(int indexLinea,int indexPunto)const{
    return tabella.at(indexLinea).at(indexPunto).first.x();
}

int DatiGrafico::getY(int indexLinea,int indexPunto)const{
    return tabella.at(indexLinea).at(indexPunto).first.y();
}

QString DatiGrafico::getNome(int indexLinea,int indexPunto)const{
    return tabella.at(indexLinea).at(indexPunto).second;
}

DatiPunto DatiGrafico::getPunto(int indexLinea,int indexPunto)const{
    return tabella.at(indexLinea).at(indexPunto);
}

DatiLinea DatiGrafico::getLinea(int indexLinea)const{
    return tabella.at(indexLinea);
}
*/

/*
DatiTabella DatiGrafico::Tabella() const{
    return tabella;
}

DatiTabella DatiGrafico::modifica(){
    return tabella;
}
*/

DatiGrafico::TipoGrafico DatiGrafico::getTipo()const{
    return tipo;
}

QString DatiGrafico::getTitolo()const{
    return titolo;
}

QString DatiGrafico::getIntestazione()const{
    return intestazione;
}
/*
void DatiGrafico::setPunto(int x,int y,int indexLinea,int indexPunto){
    if(indexLinea<tabella.count())
        if(indexPunto<tabella.at(indexLinea).count()){
            tabella[indexLinea][indexPunto].first.setX((qreal)x);
            tabella[indexLinea][indexPunto].first.setY((qreal)y);
        }
}

void DatiGrafico::setPunto(QString n,int indexLinea,int indexPunto){
    if(indexLinea<tabella.count())
        if(indexPunto<tabella.at(indexLinea).count())
            tabella[indexLinea][indexPunto].second = n;
}

void DatiGrafico::setPunto(QString n,int x,int y,int indexLinea,int indexPunto){
    if(indexLinea<tabella.count())
        if(indexPunto<tabella.at(indexLinea).count()){
            tabella[indexLinea][indexPunto].first.setX((qreal)x);
            tabella[indexLinea][indexPunto].first.setY((qreal)y);
            tabella[indexLinea][indexPunto].second = n;
        }
}

void DatiGrafico::setPunto(DatiPunto& d,int indexLinea,int indexPunto){
    if(indexLinea<tabella.count())
        if(indexPunto<tabella.at(indexLinea).count())
            tabella[indexLinea][indexPunto] = d;
}

void DatiGrafico::setLinea(QString s, int indexLinea){
    if(indexLinea<tabella.count())
        for(int i=0;i<tabella.at(indexLinea).count();++i)
            tabella[indexLinea][i].second = s;
}

void DatiGrafico::setLinea(DatiLinea& l,int indexLinea){
    if(indexLinea<tabella.count())
        if(l.count()==tabella.at(indexLinea).count())
            for(int i=0;i<tabella.at(indexLinea).count();++i)
                tabella[indexLinea][i] = l.at(i);
}
*/
void DatiGrafico::setTipo(TipoGrafico t){
    tipo = t;
}

void DatiGrafico::setTitolo(QString s){
    titolo = s;
}

void DatiGrafico::setIntestazione(QString i){
    intestazione = i;
}

DatiTabella DatiGrafico::generateRandomData(int listCount, int valueMax, int valueCount) const{
    DatiTabella dataTable;

    // set seed for random stuff
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

    // generate random data
    for (int i(0); i < listCount; i++) {
        DatiLinea dataList;
        qreal yValue(0);
        for (int j(0); j < valueCount; j++) {
            yValue = yValue + (qreal)(qrand() % valueMax) / (qreal) valueCount;
            QPointF value((j + (qreal) rand() / (qreal) RAND_MAX) * ((qreal) valueMax / (qreal) valueCount)
                          ,yValue);
            QString label = "Slice " + QString::number(i) + ":" + QString::number(j);
            dataList << DatiPunto(value, label);
        }
        dataTable << dataList;
    }

    return dataTable;
}
