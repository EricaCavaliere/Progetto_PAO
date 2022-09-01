#ifndef MISCELA_H
#define MISCELA_H

#include "campione.h"
#include "composto.h"

class Miscela: virtual public Campione
{
private:
    class Nodo{
    public:
        Composto info;
        Nodo *next, *prev;
        Nodo(const Composto&, Nodo* = nullptr, Nodo* = nullptr);
    };

    bool soluzione;
    Nodo *last, *first;
    static Nodo* copia(Nodo*,Nodo*&);
    static void distruggi(Nodo*);
public:
    class iterator{
        friend class Miscela;
    private:
        Nodo* nodo;
        bool pastTheEnd;
        iterator(Nodo*,bool=false);
    public:
        iterator();
        iterator(const iterator&);
        Composto& operator*()const;
        Composto* operator->()const;
        bool operator==(const iterator&)const;
        bool operator!=(const iterator&)const;
        iterator& operator++();
        iterator operator++(int);
        iterator& operator--();
        iterator operator--(int);
    };

    class const_iterator{
        friend class Miscela;
    private:
        Nodo* nodo;
        bool pastTheEnd;
        const_iterator(Nodo*,bool=false);
    public:
        const_iterator();
        const_iterator(const const_iterator&);
        const Composto& operator*()const;
        const Composto* operator->()const;
        bool operator==(const const_iterator&)const;
        bool operator!=(const const_iterator&)const;
        const_iterator& operator++();
        const_iterator operator++(int);
        const_iterator& operator--();
        const_iterator operator--(int);
    };

    Miscela(std::string="",Stato=indefinito, double=0.0, double=0.0, double=0.0, bool=true);
    Miscela(const Miscela&);
    Miscela& operator=(const Miscela&);
    ~Miscela();

    bool isSoluzione()const;
    void setSoluzione(bool);

    double massaSoluto(const Composto&)const;
    double volumeSoluto(const Composto&)const;
    double moliSoluto(const Composto&)const;
    double percentualeMassa(const Composto&)const;
    double percentualeVolume(const Composto&)const;
    double massaSuVolume(const Composto&)const;
    double molarita(const Composto&)const;

    unsigned int size()const;
    bool empty()const;
    Composto& operator[](unsigned int)const;
    iterator begin();
    iterator end();
    const_iterator begin()const;
    const_iterator end()const;
    void push_front(const Composto&);
    void push_back(const Composto&);
    void pop_front();
    void pop_back();
    void erase(unsigned int);
    void erase(const iterator&);
};

#endif // MISCELA_H
