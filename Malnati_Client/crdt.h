#ifndef CRDT_H
#define CRDT_H

#include <string>
#include <vector>
#include "symbol.h"


class Crdt
{ //come si mettono in mezzo?
private:
    int siteId;
    int counter;
    std::vector<Symbol> symbols;
    int compare(Symbol s1, Symbol s2);

public:
    Crdt();
    Symbol localInsert(char value, int preceding, int following);
    void localErase(Symbol symbolToErase, int position); //da capire quale dei due serve, forse si può fare senza passare il symbol
    int getSiteId();
    int getCounter();
    int getCounterAndIncrement();
    int remoteinsert(Symbol s);
    int remotedelete(Symbol s);

    std::vector<Symbol> getSymbols();
};

#endif // CRDT_H
