#include "crdt.h"
#include "symbol.h"
#include <iostream>
#include <QtDebug>

#define MAXNUM 100

bool exists(int index, std::vector<int> vector);
void resetVal();
int i=0, flag=0;
std::vector<int> tmp;

void print(std::vector<int> const &input)
{
    for (int i = 0; i < input.size(); i++) {
        //std::cout << input.at(i) << ' ';
        qDebug() << "pos: " << input.at(i);
    }
}


std::vector<int> createFractional(std::vector<int> preceding, std::vector<int> following){
    int prec, foll,flag=0;
    std::vector<int> tmp;
    unsigned long long p;
    //inizio modifiche //////////////////////////////////////////////////
   for(p=0;p<qMin(preceding.size(),following.size());p++){
       int diff=following.at(p)-preceding.at(p);
       if(diff==0){
           tmp.push_back(following.at(p));
       }
       else if(diff>1){
           if(p==preceding.size()){
               tmp.push_back((following.at(p)+preceding.at(p))/2);
           }
           else{
               for(int t=p;t<preceding.size();t++){
                   tmp.push_back(preceding.at(t));
               }
               tmp.push_back(MAXNUM/2);
           }
           flag=1;
       }
       else if(diff==1){
           if(p==preceding.size()){
               tmp.push_back(MAXNUM/2);
           }
           else{
               for(int t=p;t<preceding.size();t++){
                   tmp.push_back(preceding.at(t));
               }
               tmp.push_back(MAXNUM/2);
           }
           flag=1;
       }
   }
   if(flag==0){
       tmp.push_back(following.at(p));
   }
   return tmp;
//fine modifiche ////////////////////////////////////////////////////////////////////////
}

Crdt::Crdt()
{

}

Symbol Crdt::localInsert(char value, int precedingC, int followingC){
    //mi da la dimensione del mio vettore di simboli
    //int symbolsSize = this.symbols.size();
    size_t symbolsSize = this->symbols.size();

    //prendo il simbolo nuovo
    //Symbol symbolToInsert(value, this->getSiteId(), this->getCounterAndIncrement());
    Symbol symbolToInsert(value, std::vector<int>{0}, this->getSiteId(), this->getCounter());

    std::vector<int> fractionalPos={0};

    if(precedingC==-1){
        qDebug() << "sono all'inserimento in testa";
        fractionalPos = std::vector<int>{MAXNUM/2};
        symbolToInsert.setPosizione(fractionalPos);
    }
    else{ //caso generale?
        if(followingC == (int)symbolsSize){ //è il secondo. no caso no elemtni a destra!
                std::vector<int> preceding = this->symbols[precedingC].getPosizione();
                std::vector<int> following{MAXNUM};
                //fractionalPos = createFractional(preceding, following);
                fractionalPos = {this->symbols.back().getPosizione()[0]+1};
                qDebug() << "fractionalpos: " << fractionalPos;
                symbolToInsert.setPosizione(fractionalPos);
                resetVal();
        }
        else{
            //mi salvo le posizioni frazionarie di quello prima e di quello dopo
            std::vector<int> preceding = this->symbols[precedingC].getPosizione();
            std::vector<int> following = this->symbols[followingC].getPosizione();
            fractionalPos = createFractional(preceding, following);
//            fractionalPos = createFractional({0,0}, {1,0});
//            resetVal();
//            fractionalPos = createFractional({0,0}, {0,1});
//            resetVal();
//            fractionalPos = createFractional({0,1}, {1,2});
//            resetVal();
//            fractionalPos = createFractional({0,0}, {0,0,3});
//            resetVal();
//            fractionalPos = createFractional({0,5}, {2,5});
//            resetVal();
//            fractionalPos = createFractional({0,0}, {0,0,0,3});
//            resetVal();

            qDebug() << "fractionalpos: " << fractionalPos;
            symbolToInsert.setPosizione(fractionalPos);
            resetVal();
        }
    }

    this->symbols.insert(this->symbols.begin()+followingC, symbolToInsert);

    print(fractionalPos);
    resetVal();
    return symbolToInsert;
}

//MAI USATA
bool exists(int index, std::vector<int> vector){
    if( index < static_cast<int>(vector.size()) )
        return true;
    else return false;
}

void resetVal(){
    i=0;
    tmp.clear();
    flag=0;
}


void Crdt::localErase(Symbol symbolToErase, int position){
    /*for( std::vector<Symbol>::iterator i = this->symbols.begin(); i!=this->symbols.end(); ++i){
        if(*i==symbolToErase)
            this->symbols.erase(i);
    }
    return;*/

    std::vector<Symbol>::iterator i = this->symbols.begin()+position;
    this->symbols.erase(i);
}

int Crdt::getSiteId(){
    return this->siteId;
}

int Crdt::getCounter(){
    return this->counter;
}

std::vector<Symbol> Crdt::getSymbols(){
    return this->symbols;
}

int Crdt::getCounterAndIncrement(){
    return ++this->counter;
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


// modificare in operatore
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
