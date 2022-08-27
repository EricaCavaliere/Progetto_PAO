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

    DatiGrafico(int numP=0, int vMax=0, int numL=0,TipoGrafico=linea,QString="Grafico");
    DatiGrafico(DatiGrafico&);
    ~DatiGrafico();

    int getX(int indexLinea,int indexPunto)const;
    int getY(int indexLinea,int indexPunto)const;
    QString getNome(int indexLinea,int indexPunto)const;
    DatiPunto getPunto(int indexLinea,int indexPunto)const;
    DatiLinea getLinea(int indexLinea)const;
    DatiTabella getTabella()const; //forse da eliminare... //forse no!
    TipoGrafico getTipo()const;
    QString getTitolo()const;

    void setPunto(int x,int y,int indexLinea,int indexPunto);
    void setPunto(QString n,int indexLinea,int indexPunto);
    void setPunto(QString n,int x,int y,int indexLinea,int indexPunto);
    void setPunto(DatiPunto& d,int indexLinea,int indexPunto);
    void setLinea(QString nome, int indexLinea);
    void setLinea(DatiLinea& l,int indexLinea);
    void setTitolo(QString);

    void addLinea(DatiLinea&);
    void removeLinea();
    void removeLinea(int indexLinea);

    int count()const;
    int count(int indexLinea)const;

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
