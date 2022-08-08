#include "campione.h"

Campione::Campione(std::string n, Stato s, double m, double v, double t):
    nome(n), materia(s), massa(m), volume(v), temperatura(t){}

Campione::Campione(const Campione &c):
    nome(c.nome), materia(c.materia), massa(c.massa), volume(c.volume), temperatura(c.temperatura){}

Campione::~Campione(){}

std::string Campione::getNome()const{
    return nome;
}

Campione::Stato Campione::getMateria()const{
    return materia;
}

double Campione::getMassa()const{
    return massa;
}

double Campione::getVolume()const{
    return volume;
}

double Campione::getTemperatura()const{
    return temperatura;
}

void Campione::setNome(std::string n){
    nome = n;
}

void Campione::setMateria(Stato m){
    materia = m;
}
void Campione::setMassa(double m){
    massa = m;
}

void Campione::setVolume(double v){
    volume = v;
}

void Campione::setTemperatura(double t){
    temperatura = t;
}
