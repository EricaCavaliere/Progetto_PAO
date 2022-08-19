#ifndef DATIGRAFICO_H
#define DATIGRAFICO_H

#include <QPointF>
#include <QPair>
#include <QVector>
#include <QMap>

typedef QPair<QPointF, QString> DatiPunto;
typedef QVector<DatiPunto> DatiLinea;
typedef QVector<DatiLinea> DatiTabella;

class DatiGrafico
{
public:
    enum TipoGrafico{linea=0,punti=1,torta=2};

    DatiGrafico(int=0,int=0,int=0,TipoGrafico=linea,QString="Grafico");
    DatiGrafico(DatiGrafico&);
    ~DatiGrafico();

    int getX(int,int)const;
    int getY(int,int)const;
    QString getNome(int,int)const;
    DatiPunto getPunto(int,int)const;
    DatiLinea getLinea(int)const;
    DatiTabella getTabella()const; //forse da eliminare... //forse no!
    TipoGrafico getTipo()const;
    QString getTitolo()const;

    void setPunto(int,int,int,int);
    void setPunto(QString,int,int);
    void setPunto(QString,int,int,int,int);
    void setPunto(DatiPunto&,int,int);
    void setLinea(QString,int);
    void setLinea(DatiLinea&,int);
    void setTitolo(QString);

    int count()const;
    int count(int)const;

    DatiTabella generateRandomData(int,int,int) const;
private:
    int nPunti;
    int valoreMax;
    int nLinee;
    DatiTabella tabella;
    TipoGrafico tipo;
    QString titolo;
};

#endif // DATIGRAFICO_H
