#include "composto.h"


/** Costruttore della classe Nodo **/

Composto::Nodo::Nodo(const Elemento& e, Nodo* n, Nodo* p):info(e),next(n),prev(p){}



/** Metodi della classe iterator **/

Composto::iterator::iterator(Nodo* n,bool pte):nodo(n),pastTheEnd(pte){}

Composto::iterator::iterator():nodo(nullptr),pastTheEnd(false){}

Composto::iterator::iterator(const iterator& i):nodo(i.nodo),pastTheEnd(i.pastTheEnd){}

Elemento& Composto::iterator::operator*()const{
    return nodo->info;
}

Elemento* Composto::iterator::operator->()const{
    return &(nodo->info);
}

bool Composto::iterator::operator==(const iterator& i)const{
    return nodo==i.nodo;
}

bool Composto::iterator::operator!=(const iterator& i)const{
    return nodo!=i.nodo;
}

Composto::iterator& Composto::iterator::operator++(){
    if(nodo!=nullptr && !pastTheEnd){
        if(nodo->next == nullptr){
            nodo+=1;
            pastTheEnd = true;
        }
        else nodo = nodo->next;
    }
    return *this;
}

Composto::iterator Composto::iterator::operator++(int){
    iterator aux(*this);
    if(nodo!=nullptr && !pastTheEnd){
        if(nodo->next == nullptr){
            nodo+=1;
            pastTheEnd = true;
        }else nodo = nodo->next;
    }
    return aux;
}

Composto::iterator& Composto::iterator::operator--(){
    if(nodo!=nullptr){
        if(pastTheEnd){
            nodo-=1;
            pastTheEnd = false;
        }else nodo = nodo->prev;
    }
    return *this;
}

Composto::iterator Composto::iterator::operator--(int){
    iterator aux = *this;
    if(nodo!=nullptr){
        if(pastTheEnd){
            nodo-=1;
            pastTheEnd = false;
        }else nodo = nodo->prev;
    }
    return aux;
}



/** Metodi della classe const_iterator **/

Composto::const_iterator::const_iterator(Nodo* n,bool pte):nodo(n),pastTheEnd(pte){}

Composto::const_iterator::const_iterator():nodo(nullptr),pastTheEnd(false){}

//questo non c'e'
Composto::const_iterator::const_iterator(const const_iterator& i):nodo(i.nodo),pastTheEnd(i.pastTheEnd){}

const Elemento& Composto::const_iterator::operator*()const{
    return nodo->info;
}

const Elemento* Composto::const_iterator::operator->()const{
    return &(nodo->info);
}

bool Composto::const_iterator::operator==(const const_iterator& i)const{
    return nodo==i.nodo;
}

bool Composto::const_iterator::operator!=(const const_iterator& i)const{
    return nodo!=i.nodo;
}

Composto::const_iterator& Composto::const_iterator::operator++(){
    if(nodo!=nullptr && !pastTheEnd){
        if(nodo->next == nullptr){
            nodo+=1;
            pastTheEnd = true;
        }
        else nodo = nodo->next;
    }
    return *this;
}

Composto::const_iterator Composto::const_iterator::operator++(int){
    const_iterator aux = *this;
    if(nodo!=nullptr && !pastTheEnd){
        if(nodo->next == nullptr){
            nodo+=1;
            pastTheEnd = true;
        }else nodo = nodo->next;
    }
    return aux;
}

Composto::const_iterator& Composto::const_iterator::operator--(){
    if(nodo!=nullptr){
        if(pastTheEnd){
            nodo-=1;
            pastTheEnd = false;
        }else nodo = nodo->prev;
    }
    return *this;
}

Composto::const_iterator Composto::const_iterator::operator--(int){
    const_iterator aux(*this);
    if(nodo!=nullptr){
        if(pastTheEnd){
            nodo-=1;
            pastTheEnd = false;
        }else nodo = nodo->prev;
    }
    return aux;
}



/** Metodi della classe Composto **/

Composto::Nodo* Composto::copia(Nodo* n,Nodo* &l){
    if(n==nullptr){
        l = nullptr;
        return nullptr;
    }
    if(n->next==nullptr){
        l = new Nodo(n->info,nullptr,nullptr);
        return l;
    }
    Nodo* aux = new Nodo(n->info,copia(n->next,l),nullptr);
    aux->next->prev = aux;
    return aux;
}

void Composto::distruggi(Nodo* n){
    if(n!=nullptr){
        distruggi(n->next);
        delete n;
    }
}

bool Composto::uguale(Nodo* n1, Nodo* n2){
    if(n1==nullptr){
        if(n2==nullptr) return true;
        else return false;
    }
    return n1->info==n2->info && uguale(n1->next,n2->next);
}

Composto::Composto(std::string f, std::string n, Stato s, double m, double v, double t, unsigned int nm):
    Sostanza(n,s,m,v,t,nm),formulaChimica(f),last(nullptr),first(nullptr){}

Composto::Composto(const Composto &c):
    Sostanza(c.getNome(),c.getMateria(),c.getMassa(),c.getVolume(),c.getTemperatura(),c.getNMoli()),formulaChimica(c.formulaChimica),last(nullptr),first(copia(c.first,last)){}

Composto& Composto::operator=(const Composto& c){
    if(this!=&c){
        Sostanza::operator=(c);
        distruggi(first);
        first = copia(c.first,last);
    }
    return *this;
}

Composto::~Composto(){
    if(first!=nullptr) distruggi(first);
}

bool Composto::operator==(const Composto& c)const{
    return formulaChimica==c.formulaChimica && uguale(first,c.first)
            && getNome()==c.getNome() && getMateria()==c.getMateria() && getMassa()==c.getMassa() && getVolume()==c.getVolume() && getTemperatura()==c.getTemperatura();
}

std::string Composto::getFormulaChimica()const{
    return formulaChimica;
}

void Composto::setFormulaChimica(std::string fc){
    formulaChimica = fc;
}

double Composto::massaElemento(const Elemento& e)const{
    Nodo *aux = first;
    double massaElemento = 0.0;
    while(aux!=nullptr){
        if(aux->info == e)
            massaElemento += aux->info.getNumMassa();
        aux = aux->next;
    }
    return massaElemento;
}

double Composto::percentualeMassa(const Elemento& e)const{
    if(getMassa()==0.0) return 0.0;
    return (massaElemento(e)/getMassa())*100;
}

double Composto::massaMolecolare()const{
    Nodo *aux = first;
    double somma = 0.0;
    while(aux != nullptr){
        somma += aux->info.getNumMassa();
        aux = aux->next;
    }
    return somma;
}

unsigned int Composto::size()const{
    Nodo *aux = first;
    unsigned int count = 0;
    while(aux!=nullptr){
        count++;
        aux = aux->next;
    }
    return count;
}

bool Composto::empty()const{
    return first==nullptr;
}

Elemento& Composto::operator[](unsigned int pos)const{
    if(pos<size()){
        Nodo* aux = first;
        for(unsigned int i=0;i<pos;++i) aux = aux->next;
        return aux->info;
    }else throw std::string("Fuori dalla lista");
}

Composto::iterator Composto::begin(){
    return iterator(first);
}

Composto::iterator Composto::end(){
    if(first==nullptr) return iterator(nullptr,false);
    return iterator(last+1,true);
}

Composto::const_iterator Composto::begin()const{
    return const_iterator(first);
}

Composto::const_iterator Composto::end()const{
    if(first==nullptr) return const_iterator(nullptr,false);
    return const_iterator(last+1,true);
}

void Composto::push_front(const Elemento& e){
    if(empty()){
        first = new Nodo(e);
        last = first;
    }else{
        first->prev = new Nodo(e,first,0);
        first = first->prev;
    }
}

void Composto::push_back(const Elemento& e){
    if(empty()){
        first = new Nodo(e);
        last = first;
    }else{
        last->next = new Nodo(e,0,last);
        last = last->next;
    }
}

void Composto::pop_front(){
    if(!empty()){
        if(first==last){
            delete first;
            first = nullptr;
            last = nullptr;
        }else{
            Nodo* aux = first;
            first = first->next;
            first->prev = nullptr;
            delete aux;
        }
    }
}

void Composto::pop_back(){
    if(!empty()){
        if(first==last){
            delete first;
            first = nullptr;
            last = nullptr;
        }else{
            Nodo* aux = last;
            last = last->prev;
            last->next = nullptr;
            delete aux;
        }
    }
}

void Composto::erase(unsigned int pos){
    if(!empty() && pos<size()){
        Nodo* aux = first;
        for(unsigned int i=0;i<pos;++i) aux = aux->next;

        if(aux->prev==nullptr) first = aux->next;
        else aux->prev->next = aux->next;

        if(aux->next==nullptr) last = aux->prev;
        else aux->next->prev = aux->prev;

        delete aux;
    }else throw std::string("Fuori dalla lista");
}

void Composto::erase(const iterator& i){
    if(!empty()){
        iterator it = begin();
        while(it!=end() && it!=i) ++it;
        if(it!=i) throw std::string("Iterator non valido");

        if(it.nodo->prev==nullptr) first = it.nodo->next;
        else it.nodo->prev->next = it.nodo->next;

        if(it.nodo->next==nullptr) last = it.nodo->prev;
        else it.nodo->next->prev = it.nodo->prev;

        delete it.nodo;
    }
}
