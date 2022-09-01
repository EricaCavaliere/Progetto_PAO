#ifndef CAMPIONE_H
#define CAMPIONE_H

#include "iostream"
#include <string.h>

class Campione
{
public:
    enum Stato{solido=1,liquido=2,gassoso=3,indefinito=-1};

    Campione(std::string="",Stato=indefinito, double=0.0, double=0.0, double=0.0);
    Campione(const Campione&); //se astratta, da sostituire con clone()
    virtual ~Campione();

    std::string getNome()const;
    Stato getMateria()const;
    double getMassa()const;
    double getVolume()const;
    double getTemperatura()const;

    virtual void setNome(std::string);
    virtual void setMateria(Stato);
    virtual void setMassa(double);
    virtual void setVolume(double);
    virtual void setTemperatura(double);

private:
    std::string nome;
    Stato materia;
    double massa, volume, temperatura;
};

#endif // CAMPIONE_H
