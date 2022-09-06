#ifndef COMPOSTO_H
#define COMPOSTO_H

#include "sostanza.h"
#include "elemento.h"
#include <string.h>

class Composto: virtual public Sostanza
{
private:
    class Nodo{
    public:
        Elemento info;
        Nodo *next, *prev;
        Nodo(const Elemento&, Nodo* = nullptr, Nodo* = nullptr);
    };

    std::string formulaChimica;
    Nodo *last, *first;
    static Nodo* copia(Nodo*,Nodo*&);
    static void distruggi(Nodo*);
    static bool uguale(Nodo*,Nodo*);
public:
    class iterator{
        friend class Composto;
    private:
        Nodo* nodo;
        bool pastTheEnd;
        iterator(Nodo*,bool=false);
    public:
        iterator();
        iterator(const iterator&);
        Elemento& operator*()const;
        Elemento* operator->()const;
        bool operator==(const iterator&)const;
        bool operator!=(const iterator&)const;
        iterator& operator++();
        iterator operator++(int);
        iterator& operator--();
        iterator operator--(int);
    };

    class const_iterator{
        friend class Composto;
    private:
        Nodo* nodo;
        bool pastTheEnd;
        const_iterator(Nodo*,bool=false);
    public:
        const_iterator();
        const_iterator(const const_iterator&);
        const Elemento& operator*()const;
        const Elemento* operator->()const;
        bool operator==(const const_iterator&)const;
        bool operator!=(const const_iterator&)const;
        const_iterator& operator++();
        const_iterator operator++(int);
        const_iterator& operator--();
        const_iterator operator--(int);
    };

    Composto(std::string="",std::string="",Stato=indefinito, double=0.0, double=0.0, double=0.0,double=0.0);
    Composto(const Composto&);
    virtual Composto* clone()const;
    Composto& operator=(const Composto&);
    virtual ~Composto();

    bool operator==(const Composto&)const;

    std::string getFormulaChimica()const;
    void setFormulaChimica(std::string);

    double massaElemento(const Elemento&)const;
    double percentualeMassa(const Elemento&)const;
    double massaMolecolare()const;

    unsigned int size()const;
    bool empty()const;
    Elemento& operator[](unsigned int)const;
    iterator begin();
    iterator end();
    const_iterator begin()const;
    const_iterator end()const;
    void push_front(const Elemento&);
    void push_back(const Elemento&);
    void pop_front();
    void pop_back();
    void erase(unsigned int);
    void erase(const iterator&);
};

#endif // COMPOSTO_H
