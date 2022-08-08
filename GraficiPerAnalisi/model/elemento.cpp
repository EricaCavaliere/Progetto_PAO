#include "elemento.h"

Elemento::Elemento(int na, double nm):numAtomico(na),numMassa(nm){}

Elemento::Elemento(const Elemento& e):numAtomico(e.numAtomico),numMassa(e.numMassa){}

Elemento::~Elemento(){}

bool Elemento::operator==(const Elemento& e) const{
    return numAtomico==e.numAtomico && numMassa==e.numMassa;
}

unsigned int Elemento::getNumAtomico()const{
    return numAtomico;
}

double Elemento::getNumMassa()const{
    return numMassa;
}

void Elemento::setNumAtomico(unsigned int na){
    numAtomico = na;
}

void Elemento::setNumMassa(double nm){
    numMassa = nm;
}
