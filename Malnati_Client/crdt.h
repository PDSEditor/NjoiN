#ifndef CRDT_H
#define CRDT_H

#include <string>
#include <vector>


class Crdt
{ //come si mettono in mezzo?
private:
    int siteId;
    int counter;
    std::vector<Symbol> symbols;

public:
    Crdt();
    void localInsert(char value, int index);
    void localErase();
    int getSiteId();
    int getCounter();
    //int getCounterAndIncrement();

    std::vector<Symbol> getSymbols();
};

#endif // CRDT_H
