#include <iostream>
#include <QtDebug>
#include "Crdt.h"
#include "symbol.h"


int Crdt::maxnum=100;
int Crdt::counter=0;
 extern socketManager *sock;

bool exists(int index, std::vector<int> vector);

void print(const std::vector<int>& input)
{
    for(auto i: input){
        qDebug() << "pos: " << i;
    }
}

Crdt::Crdt() //da prendere dal server?
{


}

std::vector<int> createFractional(std::vector<int> preceding, std::vector<int> following, std::vector<int> &tmp, const int maxnum){
    int prec, foll;

    prec=preceding.at(0);
    foll=following.at(0);

    int diff=std::abs(foll-prec);

    if(diff==1 || diff==0){
        if(preceding.size()==1){
            preceding.at(0)=0;
        }
        else{
            preceding.erase(preceding.begin());
        }
        if(following.size()==1){
            following.at(0)=maxnum;
        }
        else{
            following.erase(following.begin());
        }

        createFractional(preceding, following, tmp, maxnum);
    }
    else if(diff > 1){
        tmp.push_back(prec+(foll-prec)/2);
        return tmp;
    }

    tmp.insert(tmp.begin(), prec);          //inserimento a ritroso
    return tmp;
}

/*********************************
 * si può ancora EVITARE di avere i due vect temp following e preceding
 * passando this->symbols[precedingC].getPosizione() per valore
 * ******************************/
Message Crdt::localInsert(char value, int precedingC, int followingC){
    //mi da la dimensione del mio vettore di simboli
    //int symbolsSize = this.symbols.size();
    size_t symbolsSize = this->symbols.size();

    std::vector<int> following;
    std::vector<int> preceding;
    std::vector<int> fractionalPos;

    //prendo il simbolo nuovo
    //Symbol symbolToInsert(value, std::vector<int>{0}, this->getSiteId(), this->getCounter());

    if(precedingC==-1){
        qDebug() << "sono all'inserimento in testa";

        if(symbolsSize==0){ //primissimo inserimento
            fractionalPos = std::vector<int>{maxnum/2};
        }
        else{
            following = this->symbols[followingC].getPosizione();
            //fractionalPos = createFractional({0}, following, tmp, Crdt::maxnum);
            createFractional({0}, following, fractionalPos, Crdt::maxnum);
        }
    }
    else{
        if(followingC == (int)symbolsSize){         // inserimento in coda
                preceding = this->symbols[precedingC].getPosizione();
                following=std::vector<int>{maxnum};
//                fractionalPos = createFractional(preceding, following, tmp, Crdt::maxnum);
                createFractional(preceding, following, fractionalPos, Crdt::maxnum);
                qDebug() << "fractionalpos: " << fractionalPos;
                //symbolToInsert.setPosizione(fractionalPos);
                //tmp.clear();
        }
        else{ // cioè sono in mezzo
            //mi salvo le posizioni frazionarie di quello prima e di quello dopo
            preceding = this->symbols[precedingC].getPosizione();
            following = this->symbols[followingC].getPosizione();
//            fractionalPos = createFractional(preceding, following, tmp, Crdt::maxnum);
            createFractional(preceding, following, fractionalPos, Crdt::maxnum);
            /*
            fractionalPos = createFractional({3,99}, {4,5}, tmp, Crdt::maxnum);
            tmp.clear();
            fractionalPos = createFractional({0,1}, {0,1,1}, tmp, Crdt::maxnum);
            tmp.clear();
            fractionalPos = createFractional({0,2}, {0,2,1}, tmp, Crdt::maxnum);
            tmp.clear();
            fractionalPos = createFractional({0,0}, {0,0,1}, tmp, Crdt::maxnum);
            tmp.clear();
            fractionalPos = createFractional({0,0}, {0,0,50}, tmp, Crdt::maxnum);
            tmp.clear();
            fractionalPos = createFractional({0,3,4,5}, {0,5,3,27}, tmp, Crdt::maxnum);
            tmp.clear();
            fractionalPos = createFractional({0,3,1}, {0,3,2}, tmp, Crdt::maxnum);
            tmp.clear();
            fractionalPos = createFractional({0,0}, {1,0}, tmp, Crdt::maxnum);
            tmp.clear();
            fractionalPos = createFractional({0}, {0,2}, tmp, Crdt::maxnum);
            tmp.clear();
            fractionalPos = createFractional({0,1}, {1,2}, tmp, Crdt::maxnum);
            tmp.clear();
            fractionalPos = createFractional({0,0}, {0,0,3}, tmp, Crdt::maxnum);
            tmp.clear();
            fractionalPos = createFractional({0,5}, {2,5}, tmp, Crdt::maxnum);
            tmp.clear();
            fractionalPos = createFractional({0,0}, {0,0,0,3}, tmp, Crdt::maxnum);
            tmp.clear();
            fractionalPos = createFractional({2, 3, 4}, {3}, tmp, Crdt::maxnum); //{2,50}
            tmp.clear();*/

            qDebug() << "fractionalpos: " << fractionalPos;
            //symbolToInsert.setPosizione(fractionalPos);
            //tmp.clear();
        }
    }

    Symbol symbolToInsert(value, fractionalPos, this->getSiteId(), this->getCounterAndIncrement());
//
    this->symbols.insert(this->symbols.begin()+followingC, symbolToInsert);
    Message m;
    m.setAction('I');

    m.setSymbol(&symbolToInsert);
    //sock->messageToServer(m);
    //sock->binaryMessageToServer(m);
    print(fractionalPos);
    return m;
}

//MAI USATA
bool exists(int index, std::vector<int> vector){
    if( index < static_cast<int>(vector.size()) )
        return true;
    else return false;
}

void Crdt::localErase(int position){
    std::vector<Symbol>::iterator i = this->symbols.begin()+position;
    this->symbols.erase(i);
}

int Crdt::getSiteId(){
    return this->siteId;
}

int Crdt::getCounter(){
    return Crdt::counter;
}

std::vector<Symbol> Crdt::getSymbols(){
    return this->symbols;
}

int Crdt::getCounterAndIncrement(){
    return ++Crdt::counter;
}


//INIZIO PARTE LORENZO******************************************************//


int Crdt::remoteinsert(Symbol s){
    int min=0,max=symbols.size()-1,middle=(max+min)/2,pos;
    std::vector<int> index=s.getPosizione();
    std::vector<int> tmp;
    std::vector<Symbol>::iterator it;
    //controllo se è ultimo
    if(symbols[max].getPosizione().back()<index.front()){
        symbols.push_back(s);
        return max;
    }
    //controllo se è primo
    if(symbols[0].getPosizione().front()>index.back()){
        it=symbols.begin();
        symbols.insert(it,s);
        return min;
    }
    //è in mezzo
    while(max-min>1){
       pos=this->compare(s,symbols[middle]);
       if(pos>0){
           min=middle;
       }
       else if(pos<0){
           max=middle;
       }
       middle=(max+min)/2;
    }
    it=symbols.begin();
    pos=this->compare(s,symbols[min]);
    if(pos>0){
        //inserisco dopo il min
        symbols.insert(it+min+1,s);
        return min+1;
    }
    if(pos<0){
        //inserisco prima del min
        symbols.insert(it+min-1,s);
        return min-1;
    }

}

int Crdt::remotedelete(Symbol s){
    int min=0,max=symbols.size()-1,middle=(max+min)/2,pos;
    std::vector<int> index=s.getPosizione();
    std::vector<int> tmp;
    std::vector<Symbol>::iterator it;
    it=symbols.begin();
    //controllo se è ultimo
    if(this->compare(s,symbols[max])==0){
            symbols.erase(it+max);
            return max;
}
    //controllo se è primo
    if(this->compare(s,symbols[min])==0){
            symbols.erase(it+min);
            return min;
        }
    while(max-min>1){
       pos=this->compare(s,symbols[middle]);
       if(pos>0){
           min=middle;
       }
       else if(pos<0){
           max=middle;
       }
       if(pos==0){
           symbols.erase(it+middle);
           break;
       }
       middle=(max+min)/2;
    }
    return middle;

}

/******************************************/
/* bool operator==(Symbol s1, Symbol s2) */
/* bool operator<=(Symbol s1, Symbol s2) */
/****************************************/



void Crdt::remoteM(Message *m)
{

}

int Crdt::compare(Symbol s1, Symbol s2){
    int len1=s1.getPosizione().size();
    int len2=s2.getPosizione().size();
    int res=0;
    for(int i=0;i<std::min(len1,len2);i++){

        if(s1.getPosizione()[i]>s2.getPosizione()[i]){
            res=1;
            break;
        }
        if(s1.getPosizione()[i]<s2.getPosizione()[i]){
            res=-1;
            break;
        }
    }
    if(res==0){
        if(len1>len2){
            res=1;
        }else
        if(len1<len2){
            res=-1;
        }

    }
    return res;
}

Crdt::~Crdt(){
    //rilasciare la memoria

}

