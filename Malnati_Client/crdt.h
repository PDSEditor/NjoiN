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
    void localErase(Symbol symbolToErase);
    int getSiteId();
    int getCounter();
    int getCounterAndIncrement();
    std::vector<Symbol> remoteinsert(std::vector<Symbol> vect, Symbol s);
    std::vector<Symbol> remotedelete(std::vector<Symbol> vect, Symbol s);

    std::vector<Symbol> getSymbols();
};

#endif // CRDT_H
