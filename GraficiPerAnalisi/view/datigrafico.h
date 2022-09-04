#ifndef DATIGRAFICO_H
#define DATIGRAFICO_H

#include <QPointF>
#include <QPair>
#include <QVector>

typedef QPair<QPointF, QString> DatiPunto;
typedef QList<DatiPunto> DatiLinea;
typedef QList<DatiLinea> DatiTabella;

class DatiGrafico
{
public:
    enum TipoGrafico{linea=0,punti=1,torta=2};

    DatiGrafico(QString="Grafico",QString="Grafico",TipoGrafico=linea,int numP=10, int vMax=20, int numL=1);
    DatiGrafico(const DatiGrafico&);
    ~DatiGrafico();

    QString getTitolo()const;
    QString getIntestazione() const;
    TipoGrafico getTipo()const;

    void setTipo(TipoGrafico);
    void setTitolo(QString);
    void setIntestazione(QString);

    DatiTabella tabella;
    DatiTabella generateRandomData(int,int,int) const;
private:
    QString titolo,intestazione;
    TipoGrafico tipo;
};

#endif // DATIGRAFICO_H
