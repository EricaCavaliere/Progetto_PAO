#ifndef SOSTANZA_H
#define SOSTANZA_H

#include "campione.h"

class Sostanza: public Campione
{
private:
    double nMoli;
    double massaMolare; //massaMolare = massa/nMoli

public:
    Sostanza(std::string="",Stato=indefinito, double=0.0, double=0.0, double=0.0,double=0.0);
    virtual ~Sostanza();

    double getNMoli()const;
    double getMassaMolare()const;

    void setNMoli(double);
    virtual void setMassa(double) override;

};

#endif // SOSTANZA_H
