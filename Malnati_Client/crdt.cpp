#include "crdt.h"

#define MAXNUM 100

std::vector<int> createFractional(std::vector<int> preceding, std::vector<int> following);
bool exists(int index, std::vector<int> vector);

Crdt::Crdt()
{

}

void Crdt::localInsert(char value, int index){
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

            symbolToInsert.setFractional(newFractional);
            this->symbols.insert(this->symbols.begin()+index, symbolToInsert);
        }
        else{ //caso generale?
            std::vector<int> precedingFractional = this->symbols[index-1].getFractionalPosition();
            std::vector<int> followingFractional = this->symbols[0].getFractionalPosition();

            std::vector<int> newFractional = createFractional(precedingFractional, followingFractional);

            symbolToInsert.setFractional(newFractional);
            this->symbols.insert(this->symbols.begin()+index, symbolToInsert);
        }
    }
}

std::vector<int> createFractional(std::vector<int> preceding, std::vector<int> following){
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

}

    //following ancora ha elementi
    //TODO un controllo su following



bool exists(int index, std::vector<int> vector){
    if(index < vector.size())
        return true;
    else return false;
}
