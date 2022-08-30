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

    DatiGrafico(QString="Grafico",QString="Grafico",TipoGrafico=linea,int numP=0, int vMax=0, int numL=0);
    DatiGrafico(DatiGrafico&);
    ~DatiGrafico();

    int getX(int indexLinea,int indexPunto)const;
    int getY(int indexLinea,int indexPunto)const;
    QString getNome(int indexLinea,int indexPunto)const;
    DatiPunto getPunto(int indexLinea,int indexPunto)const;
    DatiLinea getLinea(int indexLinea)const;
    DatiTabella getTabella()const;
    TipoGrafico getTipo()const;
    QString getTitolo()const;
    QString getIntestazione() const;

    void setPunto(int x,int y,int indexLinea,int indexPunto);
    void setPunto(QString n,int indexLinea,int indexPunto);
    void setPunto(QString n,int x,int y,int indexLinea,int indexPunto);
    void setPunto(DatiPunto& d,int indexLinea,int indexPunto);
    void setLinea(QString nome, int indexLinea);
    void setLinea(DatiLinea& l,int indexLinea);
    void setTipo(TipoGrafico);
    void setTitolo(QString);
    void setIntestazione(QString);

    void addLinea(DatiLinea&);
    void removeLinea();
    void removeLinea(int indexLinea);

    int count()const;

    DatiTabella generateRandomData(int,int,int) const;
private:
    QString titolo,intestazione;
    TipoGrafico tipo;
    int nPunti;
    int valoreMax;
    int nLinee;
    DatiTabella tabella;
};

#endif // DATIGRAFICO_H
