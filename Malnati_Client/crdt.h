#ifndef CRDT_H
#define CRDT_H

#include <QObject>
#include <string>
#include <vector>
#include "symbol.h"
#include "message.h"
#include "socketmanager.h"


class socketManager;
class mainwindow;

class Crdt: public QObject
{

    Q_OBJECT
signals:
    void localM(Message *m);

public slots:
   void remoteM(Message *m);


private:
    static int maxnum;
    static int counter; //il numero dell'operazione effettuata dall'utente

    int siteId; //chi ha inserito il carattere
    std::vector<Symbol> symbols;
    int compare(Symbol s1, Symbol s2); //???


public:
    explicit Crdt();
    Message localInsert(QChar value, int preceding, int following);
    Message localErase(int position);
    int getSiteId();
    void setSiteId(int);
    int getCounter();
    int getCounterAndIncrement();
    void incrementCounter();
    int remoteinsert(Symbol s);
    int remotedelete(Symbol s);

    std::vector<Symbol> getSymbols();
    void setSymbols(std::vector<Symbol> vect);
    ~Crdt();



};

#endif // CRDT_H

