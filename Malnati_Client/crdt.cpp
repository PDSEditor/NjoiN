#include "crdt.h"
#include "symbol.h"
#include <iostream>
#include <QtDebug>

#define MAXNUM 100

std::vector<int> createFractional(std::vector<int> preceding, std::vector<int> following);
bool exists(int index, std::vector<int> vector);
void resetVal();
int i=0;
std::vector<int> tmp;

void print(std::vector<int> const &input)
{
    for (int i = 0; i < input.size(); i++) {
        //std::cout << input.at(i) << ' ';
        qDebug() << "pos: " << input.at(i);
    }
}

Crdt::Crdt()
{

}

Symbol Crdt::localInsert(char value, int precedingC, int followingC){
    //mi da la dimensione del mio vettore di simboli
    //int symbolsSize = this.symbols.size();
    std::vector<Symbol>::size_type symbolsSize = this->symbols.size();

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
                fractionalPos = createFractional(preceding, following);
                symbolToInsert.setPosizione(fractionalPos);
                resetVal();
        }
        else{
            std::vector<int> preceding = this->symbols[precedingC].getPosizione();
            std::vector<int> following = this->symbols[followingC].getPosizione();
            fractionalPos = createFractional(preceding, following);
            symbolToInsert.setPosizione(fractionalPos);
            resetVal();
        }
    }

    this->symbols.insert(this->symbols.begin()+followingC, symbolToInsert);

    print(fractionalPos);
    resetVal();
    return symbolToInsert;
}


/*void Crdt::localInsert(char value, int index){
    //std::out <<"car " << value <<std:endl;
    Symbol symbolToInsert(value, this->getSiteId(), this->getCounterAndIncrement()); //prendo il simbolo nuovo
    int symbolsSize = this->symbols.size(); //mi da la dimensione del mio vettore di simboli
    if(index==0){ //è il primo oppure ho solo elementi a dx
        if(symbolsSize == 0){ //il mio vettore di simboli è vuoto, allora è il primo
            //std::vector<int> fistElemen{MAXNUM/2};

            symbolToInsert.setFractionalPosition((std::vector<int>{MAXNUM/2})); //setto la posizione maxnum/2
            this->symbols.insert(this->symbols.begin()+index, symbolToInsert); //chiave, valore
        }
        else{ //index è zero ma vettore di simboli non vuoto, allora ho elementi a dx
            std::vector<int> followingFractional = this->symbols[0].getFractionalPosition();
            std::vector<int> appoggio{0};
            std::vector<int> newFractional = createFractional(appoggio, followingFractional);
            resetVal();

            symbolToInsert.setFraction(newFractional);
            this->symbols.insert(this->symbols.begin()+index, symbolToInsert);
        }
    }
    else{ //index!=0, quindi non è il primo
        // precedentemente if(index > symbolsSize){ //allora appendo alla fine
        if(index+1 > symbolsSize){ //allora appendo alla fine
            std::vector<int> precedingFractional = this->symbols[index-1].getFractionalPosition();
            std::vector<int> appoggio{MAXNUM}; //fittizio

            std::vector<int> newFractional = createFractional(precedingFractional, appoggio);
            resetVal();

            symbolToInsert.setFractional(newFractional);
            this->symbols.insert(this->symbols.begin()+index, symbolToInsert);
        }
        else{ //caso generale?
            std::vector<int> precedingFractional = this->symbols[index-1].getFractionalPosition();
            std::vector<int> followingFractional = this->symbols[index].getFractionalPosition();

            std::vector<int> newFractional = createFractional(precedingFractional, followingFractional);
            resetVal();

            symbolToInsert.setFractional(newFractional);
            this->symbols.insert(this->symbols.begin()+index, symbolToInsert);
        }
    }
}*/

/*std::vector<int> createFractional(std::vector<int> preceding, std::vector<int> following){
    int pSize = static_cast<int>(preceding.size());
    int fSize = static_cast<int>(following.size());
    int size = std::min(pSize, fSize);
    int i;
    int diff;

    std::vector<int> tmp{0};

    for(i=0; i<size; i++){
        diff=following[i]-preceding[i];
        if(diff > 1){
            tmp.push_back(diff/2+preceding[i]);
        }
        else{
            //se la diff non è maggiore di 1, allora è minore o uguale. In questo caso inizio con quello più piccolo per una convenzione mia
            tmp.push_back(preceding[i]);
            if(diff == 1){
                //todo piccolino, da studiare cosa fare bene in questo caso
                if(exists(i+1, preceding)){
                    //i+1 esiste?
                    tmp.push_back(preceding[i+1]+1); //[2,3][2,4][2,5] e voglio inserire tra gli ultimi 2 viene [2,3][2,4][2,4,5][2,5]
                    return tmp;
                }
                else
                    tmp.push_back(preceding[i]+1); //fede dice maxnum/2 e buonanotte
                return tmp;
            }
        }
    }

}*/

std::vector<int> createFractional(std::vector<int> preceding, std::vector<int> following){
    //int pSize = static_cast<int>(preceding.size());
    //int fSize = static_cast<int>(following.size());
    //int size = std::min(pSize, fSize);
    //int prec = preceding[i];
    //int foll = following[i];

    //std::vector<int>::iterator prec = preceding.begin()+i;


    //assumo che following e preceding non siano void
    int diff = *following.begin()+i - *preceding.begin()+i;
    if(diff > 1 ){
        tmp.push_back(diff/2+*preceding.begin()+i);
        return tmp;
    } else if(diff == 1){
        tmp.push_back(*preceding.begin()+i);/*
        if( preceding.begin()+i == preceding.end() ){
            tmp.push_back(((MAXNUM-*preceding.end())/2)+*preceding.end());
            return tmp;
        }
        else{
            if( preceding.begin()+i != preceding.end()){
                tmp.push_back((MAXNUM-*preceding.end())/2+*preceding.end());
                return tmp;
            }
        }
    }*/
        tmp.push_back(((MAXNUM-*preceding.end())/2)+*preceding.end());
        return tmp;
    }
    tmp.push_back(*preceding.begin()+i);
    i++;
    createFractional(preceding, following);
}



    //following ancora ha elementi
    //TODO un controllo su following



bool exists(int index, std::vector<int> vector){
    if( index < static_cast<int>(vector.size()) )
        return true;
    else return false;
}

void resetVal(){
    i=0;
    tmp.clear();
}


void Crdt::localErase(Symbol symbolToErase){
    for( std::vector<Symbol>::iterator i = this->symbols.begin(); i!=this->symbols.end(); ++i){
        if(*i==symbolToErase)
            this->symbols.erase(i);
    }

    return;
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
