#ifndef ELEMENTO_H
#define ELEMENTO_H

#include <string>

class Elemento
{
private:
    std::string nome;
    unsigned int numAtomico;
    double numMassa;
public:
    Elemento(std::string="",int=1,double=1.0079);
    Elemento(const Elemento& e);
    ~Elemento();

    bool operator==(const Elemento&)const;

    std::string getNome()const;
    unsigned int getNumAtomico()const;
    double getNumMassa()const;

    void setNome(std::string);
    void setNumAtomico(unsigned int);
    void setNumMassa(double);
};

#endif // ELEMENTO_H
