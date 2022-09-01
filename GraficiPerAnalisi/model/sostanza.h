#ifndef SOSTANZA_H
#define SOSTANZA_H

#include "campione.h"

class Sostanza: virtual public Campione
{
private:
    unsigned int nMoli;
    double massaMolare; //massaMolare = massa/nMoli

public:
    Sostanza(std::string="",Stato=indefinito, double=0.0, double=0.0, double=0.0,unsigned int=0);
    Sostanza(const Sostanza& s);
    ~Sostanza();

    unsigned int getNMoli()const;
    double getMassaMolare()const;

    void setNMoli(unsigned int);
    virtual void setMassa(double);

};

#endif // SOSTANZA_H
