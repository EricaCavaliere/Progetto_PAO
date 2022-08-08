#include "punto.h"

Punto::Punto():p(0.0,0.0),nome("Punto"){}

Punto::Punto(QPointF f, QString s):p(f),nome(s){}

Punto::Punto(Punto &s):p(s.p),nome(s.nome){}

Punto::~Punto(){}

QPointF Punto::getP()const{return p;}

QString Punto::getNome() const{return nome;}
