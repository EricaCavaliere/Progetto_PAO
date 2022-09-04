#include "datigrafico.h"

#include <QTime>

DatiGrafico::DatiGrafico(QString s,QString i,TipoGrafico t,int nPunti, int valoreMax, int nLinee)
    :tabella(generateRandomData(nLinee,valoreMax,nPunti)),titolo(s),intestazione(i),tipo(t){}

DatiGrafico::DatiGrafico(const DatiGrafico &d)
    :tabella(d.tabella),titolo(d.titolo),intestazione(d.intestazione),tipo(d.tipo){}

DatiGrafico::~DatiGrafico(){}

DatiGrafico::TipoGrafico DatiGrafico::getTipo()const{
    return tipo;
}

QString DatiGrafico::getTitolo()const{
    return titolo;
}

QString DatiGrafico::getIntestazione()const{
    return intestazione;
}

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
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
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
