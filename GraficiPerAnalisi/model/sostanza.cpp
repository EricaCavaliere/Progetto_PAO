#include "sostanza.h"

Sostanza::Sostanza(std::string n, Stato s, double m, double v, double t,double nm):
    Campione(n,s,m,v,t), nMoli(nm), massaMolare(nMoli>0 ? getMassa()/nMoli : 0.0){}

//Sostanza::Sostanza(const Sostanza &s):
//    Campione(s.getNome(),s.getMateria(),s.getMassa(),s.getVolume(),s.getTemperatura()), nMoli(s.nMoli), massaMolare(s.massaMolare){}

Sostanza::~Sostanza(){}

double Sostanza::getNMoli()const{
    return nMoli;
}

double Sostanza::getMassaMolare()const{
    return massaMolare;
}

void Sostanza::setNMoli(double nm){
    nMoli = nm;
    massaMolare = nMoli>0 ? getMassa()/nMoli : 0.0;
}

void Sostanza::setMassa(double m){
    Campione::setMassa(m);
    massaMolare = nMoli>0 ? getMassa()/nMoli : 0.0;
}
