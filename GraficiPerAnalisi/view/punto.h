#ifndef PUNTO_H
#define PUNTO_H

#include <QPointF>
#include <QString>

class Punto{
private:
    QPointF p;
    QString nome;
public:
    Punto();
    Punto(QPointF f,QString s="Punto");
    Punto(Punto &s);
    ~Punto();
    QPointF getP() const;
    QString getNome() const;
};

#endif // PUNTO_H
