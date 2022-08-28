#include "sostanza.h"

Sostanza::Sostanza(std::string n, Stato s, double m, double v, double t,unsigned int nm):
    Campione(n,s,m,v,t), nMoli(nm)
{
    massaMolare = (nMoli>0 ? getMassa()/nMoli : 0.0);
}

Sostanza::Sostanza(const Sostanza &s):
    Campione(s.getNome(),s.getMateria(),s.getMassa(),s.getVolume(),s.getTemperatura()), nMoli(s.nMoli), massaMolare(s.massaMolare){}

Sostanza::~Sostanza(){}

//metodi get
unsigned int Sostanza::getNMoli()const{
    return nMoli;
}

double Sostanza::getMassaMolare()const{
    return massaMolare;
}

void Sostanza::setNMoli(unsigned int nm){
    nMoli = nm;
}

void Sostanza::setMassaMolare(double m){
    massaMolare = m;
}
