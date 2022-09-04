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

    /*
    int getX(int indexLinea,int indexPunto)const;
    int getY(int indexLinea,int indexPunto)const;
    QString getNome(int indexLinea,int indexPunto)const;
    DatiPunto getPunto(int indexLinea,int indexPunto)const;
    DatiLinea getLinea(int indexLinea)const;
    */
    QString getTitolo()const;
    QString getIntestazione() const;
    TipoGrafico getTipo()const;
    //DatiTabella Tabella() const;

    //DatiTabella modifica();
    /*
    void setPunto(int x,int y,int indexLinea,int indexPunto);
    void setPunto(QString n,int indexLinea,int indexPunto);
    void setPunto(QString n,int x,int y,int indexLinea,int indexPunto);
    void setPunto(DatiPunto& d,int indexLinea,int indexPunto);
    void setLinea(QString nome, int indexLinea);
    void setLinea(DatiLinea& l,int indexLinea);
    */
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
