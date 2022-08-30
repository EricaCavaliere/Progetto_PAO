#include "miscela.h"


/** Costruttore della classe Nodo **/

Miscela::Nodo::Nodo(const Composto& c, Nodo* n, Nodo* p):info(c),next(n),prev(p){}



/** Metodi della classe iterator **/

Miscela::iterator::iterator(Nodo* n,bool pte):nodo(n),pastTheEnd(pte){}

Miscela::iterator::iterator():nodo(nullptr),pastTheEnd(false){}

Miscela::iterator::iterator(const iterator& i):nodo(i.nodo),pastTheEnd(i.pastTheEnd){}

Composto& Miscela::iterator::operator*()const{
    return nodo->info;
}

Composto* Miscela::iterator::operator->()const{
    return &(nodo->info);
}

bool Miscela::iterator::operator==(const iterator& i)const{
    return nodo==i.nodo && pastTheEnd==i.pastTheEnd;
}

bool Miscela::iterator::operator!=(const iterator& i)const{
    return nodo!=i.nodo && pastTheEnd!=i.pastTheEnd;
}

Miscela::iterator& Miscela::iterator::operator++(){
    if(nodo != nullptr){
        if(nodo->next != nullptr) nodo = nodo->next;
        else pastTheEnd = true;
    }
    return *this;
}

Miscela::iterator Miscela::iterator::operator++(int){
    iterator aux(*this);
    if(nodo != nullptr){
        if(nodo->next != nullptr) nodo = nodo->next;
        else pastTheEnd = true;
    }
    return aux;
}

Miscela::iterator& Miscela::iterator::operator--(){
    if(nodo != nullptr){
        if(nodo->prev != nullptr) nodo = nodo->prev;
        else pastTheEnd = false;
    }
    return *this;
}

Miscela::iterator Miscela::iterator::operator--(int){
    iterator aux = *this;
    if(nodo != nullptr){
        if(nodo->prev != nullptr) nodo = nodo->prev;
        else pastTheEnd = false;
    }
    return aux;
}



/** Metodi della classe const_iterator **/

Miscela::const_iterator::const_iterator(Nodo* n,bool pte):nodo(n),pastTheEnd(pte){}

Miscela::const_iterator::const_iterator():nodo(nullptr),pastTheEnd(false){}

Miscela::const_iterator::const_iterator(const const_iterator& i):nodo(i.nodo),pastTheEnd(i.pastTheEnd){}

const Composto& Miscela::const_iterator::operator*()const{
    return nodo->info;
}

const Composto* Miscela::const_iterator::operator->()const{
    return &(nodo->info);
}

bool Miscela::const_iterator::operator==(const const_iterator& i)const{
    return nodo==i.nodo && pastTheEnd==i.pastTheEnd;
}

bool Miscela::const_iterator::operator!=(const const_iterator& i)const{
    return nodo!=i.nodo && pastTheEnd!=i.pastTheEnd;
}

Miscela::const_iterator& Miscela::const_iterator::operator++(){
    if(nodo!=nullptr){
        if(nodo->next != nullptr) nodo = nodo->next;
        else pastTheEnd = true;
    }
    return *this;
}

Miscela::const_iterator Miscela::const_iterator::operator++(int){
    const_iterator aux = *this;
    if(nodo!=nullptr){
        if(nodo->next != nullptr) nodo = nodo->next;
        else pastTheEnd = true;
    }
    return aux;
}

Miscela::const_iterator& Miscela::const_iterator::operator--(){
    if(nodo!=nullptr){
        if(nodo->prev != nullptr) nodo = nodo->prev;
        else pastTheEnd = false;
    }
    return *this;
}

Miscela::const_iterator Miscela::const_iterator::operator--(int){
    const_iterator aux(*this);
    if(nodo!=nullptr){
        if(nodo->prev != nullptr) nodo = nodo->prev;
        else pastTheEnd = false;
    }
    return aux;
}



/** Metodi della classe Miscela **/

Miscela::Nodo* Miscela::copia(Nodo* n,Nodo* &l){
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

void Miscela::distruggi(Nodo* n){
    if(n!=nullptr){
        distruggi(n->next);
        delete n;
    }
}

Miscela::Miscela(std::string n, Stato s, double m, double v, double t, bool sol):Campione(n,s,m,v,t),soluzione(sol),last(nullptr),first(nullptr){}

Miscela::Miscela(const Miscela& m):
    Campione(m.getNome(),m.getMateria(),m.getMassa(),m.getVolume(),m.getTemperatura()),soluzione(m.soluzione),last(nullptr),first(copia(m.first,last)){}

Miscela& Miscela::operator=(const Miscela& m){
    if(this!=&m){
        Campione::operator=(m);
        distruggi(first);
        first = copia(m.first,last);
    }
    return *this;
}

Miscela::~Miscela(){
    if(first!=nullptr) distruggi(first);
}

bool Miscela::getSoluzione()const{
    return soluzione;
}

void Miscela::setSoluzione(bool s){
    soluzione = s;
}

//------------------------------- try-catch --------------------------------
double Miscela::massaSoluto(const Composto& c)const{
    if(!soluzione) throw std::string("Non è una soluzione");
    Nodo* aux = first;
    double massaSoluto = 0.0;
    while(aux!=nullptr){
        if(aux->info == c)
            massaSoluto += c.getMassa();
        aux = aux->next;
    }
    return massaSoluto;
}

//------------------------------- try-catch --------------------------------
double Miscela::volumeSoluto(const Composto& c)const{
    if(!soluzione) throw std::string("Non è una soluzione");
    Nodo* aux = first;
    double volumeSoluto = 0.0;
    while(aux!=nullptr){
        if(aux->info == c)
            volumeSoluto += c.getVolume();
        aux = aux->next;
    }
    return volumeSoluto;
}

//------------------------------- try-catch --------------------------------
double Miscela::moliSoluto(const Composto & c) const{
    if(!soluzione) throw std::string("Non è una soluzione");
    Nodo* aux = first;
    double moliSoluto = 0.0;
    while(aux!=nullptr){
        if(aux->info == c)
            moliSoluto += c.getNMoli();
        aux = aux->next;
    }
    return moliSoluto;
}

//------------------------------- try-catch --------------------------------
double Miscela::percentualeMassa(const Composto& c)const{
    if(!soluzione) throw std::string("Non è una soluzione");
    if(getMassa()==0.0) return 0.0;
    return (massaSoluto(c)/getMassa())*100;
}

//------------------------------- try-catch --------------------------------
double Miscela::percentualeVolume(const Composto& c)const{
    if(!soluzione) throw std::string("Non è una soluzione");
    if(getVolume()==0.0) return 0.0;
    return (volumeSoluto(c)/getVolume())*100;
}

//------------------------------- try-catch --------------------------------
double Miscela::massaSuVolume(const Composto& c)const{
    if(!soluzione) throw std::string("Non è una soluzione");
    if(getVolume()==0.0) return 0.0;
    return (massaSoluto(c)/getVolume())*100;
}

//------------------------------- try-catch --------------------------------
double Miscela::molarita(const Composto& c)const{
    if(!soluzione) throw std::string("Non è una soluzione");
    if(getVolume()==0.0) return 0.0;
    return (moliSoluto(c)/getVolume())*100;
}

unsigned int Miscela::size()const{
    Nodo *aux = first;
    unsigned int count = 0;
    while(aux!=nullptr){
        count++;
        aux = aux->next;
    }
    return count;
}

bool Miscela::empty()const{
    return first==nullptr;
}

//------------------------------- try-catch --------------------------------
Composto& Miscela::operator[](unsigned int pos)const{
    if(pos<size()){
        Nodo* aux = first;
        for(unsigned int i=0;i<pos;++i) aux = aux->next;
        return aux->info;
    }else throw std::string("Fuori dalla lista");
}

Miscela::iterator Miscela::begin(){
    return iterator(first,false);
}

Miscela::iterator Miscela::end(){
    return iterator(last,true);
}

Miscela::const_iterator Miscela::begin()const{
    return const_iterator(first,false);
}

Miscela::const_iterator Miscela::end()const{
    return const_iterator(last,true);
}

void Miscela::push_front(const Composto& e){
    if(empty()){
        first = new Nodo(e);
        last = first;
    }else{
        first->prev = new Nodo(e,first,0);
        first = first->prev;
    }
}

void Miscela::push_back(const Composto& e){
    if(empty()){
        first = new Nodo(e);
        last = first;
    }else{
        last->next = new Nodo(e,0,last);
        last = last->next;
    }
}

void Miscela::pop_front(){
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

void Miscela::pop_back(){
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

//------------------------------- try-catch --------------------------------
void Miscela::erase(unsigned int pos){
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

//------------------------------- try-catch --------------------------------
void Miscela::erase(const iterator& i){
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
