#include "elemento.h"

Elemento::Elemento(std::string s, int na, double nm):nome(s),numAtomico(na),numMassa(nm){}

Elemento::Elemento(const Elemento& e):nome(e.nome),numAtomico(e.numAtomico),numMassa(e.numMassa){}

Elemento::~Elemento(){}

bool Elemento::operator==(const Elemento& e) const{
    return numAtomico==e.numAtomico && numMassa==e.numMassa;
}

std::string Elemento::getNome()const{
    return nome;
}

unsigned int Elemento::getNumAtomico()const{
    return numAtomico;
}

double Elemento::getNumMassa()const{
    return numMassa;
}

void Elemento::setNome(std::string s){
    nome = s;
}

void Elemento::setNumAtomico(unsigned int na){
    numAtomico = na;
}

void Elemento::setNumMassa(double nm){
    numMassa = nm;
}
