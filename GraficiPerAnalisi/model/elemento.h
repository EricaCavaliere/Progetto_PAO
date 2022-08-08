#ifndef ELEMENTO_H
#define ELEMENTO_H


class Elemento
{
private:
    unsigned int numAtomico;
    double numMassa;
public:
    Elemento(int=1,double=1.0079);
    Elemento(const Elemento& e);
    ~Elemento();

    bool operator==(const Elemento&)const;

    unsigned int getNumAtomico()const;
    double getNumMassa()const;

    void setNumAtomico(unsigned int);
    void setNumMassa(double);
};

#endif // ELEMENTO_H
